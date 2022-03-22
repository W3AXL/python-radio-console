# Base libraries
import sys
import glob
import os
import argparse
import platform
import threading
import time
import json
import queue
import audioop
import ssl
from typing_extensions import runtime

# TCP Socket Server
import websockets
import websockets.exceptions
import asyncio

# HTTP server stuff
import socketserver
import http.server
import ssl

# WebRTC Stuff
from aiortc import MediaStreamTrack, RTCPeerConnection, RTCSessionDescription, logging
from aiortc.contrib.media import MediaBlackhole, MediaPlayer, MediaRecorder, AudioFrame
import uuid

# Sound stuff
import pyaudio
from mulaw import MuLaw
from av.audio.frame import AudioFrame

import fractions

# Numpy (used for sound processing)
import numpy as np

# Serial stuff
import serial
import serial.tools.list_ports

# Radio class
from radioClass import Radio

# Radio State
from radioState import RadioState

# Logger
from logger import Logger

# Used for loading config and sending messages
import json

# CPU profiling
import yappi

# Config class (loaded from JSON)
class Config():
    # Init
    def __init__(self):
        # List of radio configs
        self.RadioList = []

# Create new config
configFile = ""
config = Config()

# Argument parser
parser = argparse.ArgumentParser()

# Global variables
verbose = False
address = None
serverport = None
webguiport = None
noreset = False
profiling = False

# Websocket server and event loop
server = None
serverLoop = None

# WebRTC variables
rtcPeer = None

# Message queue for sending to client
messageQueue = asyncio.Queue()

# Audio globals
audioSampleRate = 48000    # this must match the WebRTC settings on the client (48000hz OPUS)

# frame sizes for pyaudio buffers (these were set by observing what size the aiortc frames were)
spkrBufferSize = 960
micBufferSize = 960

spkrThread = None

# Client mic input vars
micSampleQueue = queue.Queue()

# Radio spkr output vars
spkrSampleQueue = queue.Queue()

# Test input & output audio streams
micStream = None
spkrStream = None

# WebRTC Tracks
micTrack = None
spkrTrack = None
blackHole = None

# Sound device lists
inputs = []
outputs = []
hostapis = []

# pyAudio instantiation
pa = pyaudio.PyAudio()

# Detect operating system
osType = platform.system()

# Create logger object
logger = Logger()
logger.initLogs()


"""-------------------------------------------------------------------------------
    Command Line Argument Functions
-------------------------------------------------------------------------------"""

def addArguments():
    """
    Add command line arguments
    """
    parser.add_argument("-a","--address", help="Server address to bind to")
    parser.add_argument("-c","--config", help="Config file to load", metavar="config.json")
    parser.add_argument("-ls","--list-sound", help="List available sound devices", action="store_true")
    parser.add_argument("-lp","--list-ports", help="List available com ports", action="store_true")
    parser.add_argument("-nr","--no-reset", help="Don't reset connected radios on start", action="store_true")
    parser.add_argument("-sp","--serverport", help="Websocket server port")
    parser.add_argument("-v","--verbose", help="Enable verbose logging", action="store_true")
    parser.add_argument("-wc","--webguicert", help="Web GUI certificate for TLS")
    parser.add_argument("-wp","--webguiport", help="Web GUI port")
    parser.add_argument("-cp", "--cpu-profiling", help="Enable yappi CPU profiling", action="store_true")

def parseArguments():
    
    global verbose
    global address
    global serverport
    global webguiport
    global noreset
    global profiling

    # Parse the args
    args = parser.parse_args()

    # Make sure we've got a valid set of them

    # Verbose logging
    if args.verbose:
        logger.setVerbose(args.verbose)
        logger.logInfo("Verbose logging enabled")

    # List available serial devices
    if args.list_ports:
        logger.logInfo("Listing available serial ports")
        getSerialDevices()
        exit(0)

    # List available sound devices
    if args.list_sound:
        print()
        getSoundDevices()
        printSoundDevices()
        exit(0)

    # Make sure a config file was specified
    if not args.config:
        Logger.logWarn("No config file specified, exiting")
        exit(0)
    else:
        loadConfig(args.config)

    # Make sure port and optionally an address were specified
    if not args.serverport:
        logger.logError("No server port specified!")
        exit(1)
    else:
        serverport = int(args.serverport)
        if not args.address:
            address = "localhost"
        else:
            address = args.address

    if not args.webguiport:
        logger.logError("No web GUI port specified!")
        exit(1)
    else:
        webguiport = int(args.webguiport)

    if args.no_reset:
        noreset = True

    if args.cpu_profiling:
        profiling = True

"""-------------------------------------------------------------------------------
    Config Parsing Functions
-------------------------------------------------------------------------------"""

def loadConfig(filename):
    """Load JSON config file and parse to config objects

    Args:
        filename (string): filename/path

    Returns:
        bool: success or failure
    """

    try:
        # make sure file is valid
        if not os.path.exists(filename):
            raise ValueError("Specified file does not exist")

        # open and parse
        with open(filename, 'r') as inp:
            # Load JSON into dict
            configDict = json.load(inp)

            # Iterate through radios in idct
            for index, radioDict in enumerate(configDict["RadioList"]):
                config.RadioList.append(Radio.decodeConfig(index, radioDict, logger))

            # Print on success
            logger.logInfo("Sucessfully loaded config file {}".format(filename))

            # Return true
            return True
    except ValueError as ex:
        logger.logError("Error loading config file: {}".format(ex.args[0]))
        return False

def printRadios():
    logger.logInfo("Loaded radios:")
    for idx, radio in enumerate(config.RadioList):
        print("      - radio{}: {}".format(idx, radio.name))
        print("                {} control ({})".format(radio.ctrlMode, radio.ctrlPort))
        print("                Tx Audio dev: {} ({})".format(radio.txDev, getDeviceName(radio.txDev)))
        print("                Rx Audio dev: {} ({})".format(radio.rxDev, getDeviceName(radio.rxDev)))

"""-------------------------------------------------------------------------------
    Radio Functions
-------------------------------------------------------------------------------"""

def connectRadios():
    """
    Connect to each radio in the master RadioList
    """
    for idx, radio in enumerate(config.RadioList):
        # Log
        logger.logInfo("Connecting to radio {}".format(radio.name))
        # Connect
        radio.connect(radioStatusUpdate, reset = not noreset)

def radioStatusUpdate(index):
    """
    Status callback the radio interface calls when it has a new status
        simply puts the index of the radio with a new status into the status update queue

    Args:
        index (int): index of the radio in the master RadioList with a new status
    """

    # Add the index to the queue
    serverLoop.call_soon_threadsafe(messageQueue.put_nowait,"status:{}".format(index))


def setTransmit(index, transmit):
    """
    Set transmit state of radio at index

    Args:
        index (int): index of the radio
        transmit (bool): state of transmit
    """
    config.RadioList[index].transmit(transmit)


def changeChannel(index, down):
    """
    Changes the channel up or down on the radio

    Args:
        index (int): index of radio
        down ([type]): whether to go down or not
    """
    config.RadioList[index].changeChannel(down)

def toggleMonitor(index):
    """
    Toggles monitor on radio

    Args:
        index (int): Index of radio in RadioList
    """
    config.RadioList[index].toggleMonitor()

def nuisanceDelete(index):
    """
    Nuisance deletes channel on radio

    Args:
        index (int): Index of radio in RadioList
    """
    config.RadioList[index].nuisanceDelete()

def togglePower(index):
    """
    Toggle power on radio

    Args:
        index (int): Index of radio in RadioList
    """
    config.RadioList[index].togglePower()

def toggleScan(index):
    """
    Enables or disabled scan on radio

    Args:
        index (int): index of radio
    """
    config.RadioList[index].toggleScan()

def toggleDirect(index):
    """
    Toggles direct/talkaround

    Args:
        index (index): index of radio
    """
    config.RadioList[index].toggleDirect()

def toggleMute(index, state):
    """
    Set state of mute for radio at index

    Args:
        index (int): Radio index
        state (bool): state of mute
    """
    config.RadioList[index].setMute(state)

def getRadioStatusJson(index):
    """
    Gets status of specified radio index in the RadioList and returns a json string

    Args:
        index (int): index of radio in RadioList

    Returns:
        string: JSON of radio status
    """
    
    # Get the status of the specified radio
    status = config.RadioList[index].encodeClientStatus()

    return json.dumps(status)


def getAllRadiosStatusJson():
    """
    Gets status of all radios and returns a JSON string

    Returns:
        string: JSON string of all radio statuses
    """    

    # Create an empty status list
    statusList = []

    # Get the status of each radio
    for radio in config.RadioList:
        statusList.append(radio.encodeClientStatus())

    # Convert into a json string
    return json.dumps(statusList)
    
"""-------------------------------------------------------------------------------
    WebRTC Functions
-------------------------------------------------------------------------------"""

class MicStreamTrack(MediaStreamTrack):
    """
    An audio stream object for the mic audio from the client
    """
    kind = "audio"

    def __init__(self, track):
        super().__init__()
        self.track = track

    async def recv(self):

        # Get a new PyAV frame
        frame = await self.track.recv()

        # Convert to int16 numpy array and get first set of data only (the array is inside another array)
        intArray = frame.to_ndarray(dtype=np.int16)[0]

        # This array is interleaved L/R samples, so pick out only one
        monoArray = intArray[0::2].copy() 

        # Put samples to radio, if there's one transmitting
        for radio in config.RadioList:
            if radio.state == RadioState.Transmitting:
                radio.micQueue.put_nowait(monoArray)

class SpkrStreamTrack(MediaStreamTrack):
    """
    An audio stream object for the speaker data from the server
    """
    kind = "audio"

    def __init__(self):
        super().__init__()
        self.samplerate = audioSampleRate
        self.samples = spkrBufferSize
        logger.logVerbose("SpkrStreamTrack initialized")

    async def recv(self):

        # Handle timestamps properly
        if hasattr(self, "_timestamp"):
            self._timestamp += self.samples
            wait = self._start + (self._timestamp / self.samplerate) - time.time()
            await asyncio.sleep(wait)
        else:
            self._start = time.time()
            self._timestamp = 0

        # create empty data by default
        data = np.zeros(self.samples).astype(np.int16)

        # Only get speaker data if we have some in the buffer
        if spkrSampleQueue.qsize() > 0:
            try:
                data = spkrSampleQueue.get_nowait().astype(np.int16)
            except queue.Empty:
                pass

        # To convert to a mono audio frame, we need the array to be an array of single-value arrays for each sample (annoying)
        data = data.reshape(data.shape[0], -1).T
        # Create audio frame
        frame = AudioFrame.from_ndarray(data, format='s16', layout='mono')

        # Update time stuff
        frame.pts = self._timestamp
        frame.sample_rate = self.samplerate
        frame.time_base = fractions.Fraction(1, self.samplerate)

        # Return
        return frame

async def gotRtcOffer(offerObj):
    """
    Called when we receive a WebRTC offer from the client

    Args:
        offerObj (dict): WebRTC SDP offer object
    """

    global rtcPeer

    logger.logInfo("Got WebRTC offer")

    # Start audio on radios
    logger.logVerbose("Starting audio devices on radios")
    startSound()
    
    # Create SDP offer and peer connection objects
    offer = RTCSessionDescription(sdp=offerObj["sdp"], type=offerObj["type"])
    rtcPeer = RTCPeerConnection()

    # Create UUID for peer
    pcUuid = "Peer({})".format(uuid.uuid4())
    logger.logVerbose("Creating peer connection {}".format(pcUuid))

    # Create the speaker track
    spkrTrack = SpkrStreamTrack()
    rtcPeer.addTrack(spkrTrack)
    logger.logVerbose("Added speaker track")

    # ICE connection state callback
    @rtcPeer.on("iceconnectionstatechange")
    async def onIceConnectionStateChange():
        logger.logVerbose("Ice connection state is now {}".format(rtcPeer.iceConnectionState))
        if rtcPeer.iceConnectionState == "failed":
            await rtcPeer.close()
            logger.logError("WebRTC peer connection {} failed".format(pcUuid))
    
    # Audio track callback when we get the mic track from the client
    @rtcPeer.on("track")
    async def onTrack(track):

        global micTrack
        global spkrTrack
        global blackHole
        global rtcPeer

        logger.logVerbose("Got {} track from peer {}".format(track.kind, pcUuid))

        # make sure it's audio
        if track.kind != "audio":
            logger.logError("Got non-audio track from peer {}".format(pcUuid))
            return
        
        # Create the mic stream for this track, and add its output to a media blackhole so it starts
        micTrack = MicStreamTrack(track)
        blackHole = MediaBlackhole()
        blackHole.addTrack(micTrack)
        logger.logVerbose("Added mic track")
        await blackHole.start()
        logger.logVerbose("Started mic track")

        # Track ended handler
        @track.on("ended")
        async def onEnded():
            logger.logVerbose("Audio track from {} ended".format(pcUuid))

    await doRtcAnswer(offer)

    logger.logVerbose("done")

async def stopRtc():

    # Stop the peer if it's open
    logger.logVerbose("Stopping RTC connection")
    if rtcPeer:
        await rtcPeer.close()

async def doRtcAnswer(offer):
    # Handle the received offer
    logger.logVerbose("Creating remote description from offer")
    await rtcPeer.setRemoteDescription(offer)

    # Create answer
    logger.logVerbose("Creating WebRTC answer")
    answer = await rtcPeer.createAnswer()

    # Set local description
    logger.logVerbose("setting local SDP")
    await rtcPeer.setLocalDescription(answer)

    # Send answer
    logger.logVerbose("sending SDP answer")
    message = '{{ "webRtcAnswer": {{ "type": "{}", "sdp": {} }} }}'.format(rtcPeer.localDescription.type, json.dumps(rtcPeer.localDescription.sdp))
    #logger.logVerbose(message.replace("\\r\\n", "\r\n"))
    messageQueue.put_nowait(message)

"""-------------------------------------------------------------------------------
    Sound Device Functions
-------------------------------------------------------------------------------"""

def getSoundDevices():
    """
    Get available system sound devices
    """

    global inputs
    global outputs
    global hostapis

    # get portaudio info
    info = pa.get_host_api_info_by_index(0)
    numdevices = info.get('deviceCount')

    for i in range(0, numdevices):
        if (pa.get_device_info_by_host_api_device_index(0, i).get('maxInputChannels')) > 0:
            inputs.append({
                'index': i,
                'name': pa.get_device_info_by_host_api_device_index(0,i).get('name')
            })
        elif (pa.get_device_info_by_host_api_device_index(0, i).get('maxOutputChannels')) > 0:
            outputs.append({
                'index': i,
                'name': pa.get_device_info_by_host_api_device_index(0,i).get('name')
            })

def printSoundDevices():
    """
    Print queried sound devices
    """

    # Print inputs first
    logger.logInfo("Available input devices:")
    for input in inputs:
        print("{}: {}".format(input['index'],input['name']))
    # Line break
    print()
    # Print outputs
    logger.logInfo("Available output devices")
    for output in outputs:
        print("{}: {}".format(output['index'],output['name']))
    # Line break
    print()

def getDeviceName(idx):
    """
    Returns the name of the specified PortAudio device index

    Args:
        idx (int): device index

    Returns:
        string: Device name
    """
    return pa.get_device_info_by_host_api_device_index(0, idx).get('name')

def startSound():
    """
    Start audio devices for each connected radio
    """
    
    # Start audio on each radio device
    for radio in config.RadioList:
        radio.startAudio(pa, micSampleQueue, audioSampleRate, micBufferSize, spkrBufferSize)

    # Start the speaker audio handler
    spkrThread = threading.Thread(target=handleSpkrData, daemon=True)
    spkrThread.start()

def stopSound():
    """
    Stops audio on each connected radio
    """

    for radio in config.RadioList:
        radio.stopAudio()

def handleSpkrData():
    """
    This is an infinite loop, to be run in a thread

    Get the speaker data from each connected radio, add it to the buffer string, and send & clear the buffer if it's big enough
    """

    global spkrBufferSize

    while True:

        outputIntArray = None
        gotSamples = False
        
        # Get samples from each radio and add to the output array
        for radio in config.RadioList:
            if radio.state == RadioState.Receiving and not radio.muted:
                try:
                    samples = radio.spkrQueue.get_nowait()
                    if not outputIntArray:
                        gotSamples = True
                        outputIntArray = samples
                    else:
                        outputIntArray = np.add(outputIntArray, samples)
                except queue.Empty:
                    #logger.logWarn("Radio {} queue empty".format(radio.name))
                    pass

        if gotSamples:
            # Put the samples into the queue
            spkrSampleQueue.put_nowait(outputIntArray)
            
        else:
            # give the CPU a break
            time.sleep(0.01)

"""-------------------------------------------------------------------------------
    Serial Port Functions
-------------------------------------------------------------------------------"""

def getSerialDevices():
    """
    Gets a list of avaialble serial devices based on operating system
    """

    # Windows
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]

    # Linux
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        ports = glob.glob('/dev/tty[A-Za-z]*')
    
    # Fallback
    else:
        logger.logError("Unknown OS detected!")
        exit(1)

    # Find which ports are valid
    results = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            results.append(port)
        except (OSError, serial.SerialException):
            pass

    # Print the result
    for port in results:
        logger.logInfo("Port {}".format(port))


"""-------------------------------------------------------------------------------
    Websocket Server Functions
-------------------------------------------------------------------------------"""

async def websocketHandler(websocket, path):
    """
    Sets up handlers for websocket
    """

    consumer_task = asyncio.ensure_future(consumer_handler(websocket, path))

    producer_task = asyncio.ensure_future(producer_hander(websocket, path))

    done, pending = await asyncio.wait(
        [consumer_task, producer_task],
        return_when=asyncio.FIRST_COMPLETED,
    )
    for task in pending:
        task.cancel()
    

async def consumer_handler(websocket, path):
    """
    Websocket handler for data received from client

    Args:
        websocket (websocket): websocket object
        path (path): not sure what this does, we don't use it
    """

    while True:
        try:
            # Wait for data
            data = await websocket.recv()

            # Try and convert the recieved data to JSON and warn on fail
            try:
                cmdObject = json.loads(data)
            except ValueError as e:
                logger.logWarn("Invalid data recieved from client, {}\nData: {}".format(e.args[0], data))
                continue

            # Iterate through the received command keys (there should only ever be one, but it's possible to recieve multiple)

            for key in cmdObject.keys():

                #
                # Radio Query Command
                #

                if key == "radios" and cmdObject[key]["command"] == "query":
                    await messageQueue.put("allradios")

                #
                # Radio Control Commands
                #

                elif key == "radioControl":
                    # Get object inside
                    params = cmdObject[key]
                    command = params["command"]
                    index = params["index"]
                    options = params["options"]

                    # Start PTT
                    if command == "startTx":
                        logger.logVerbose("Starting TX on radio index {}".format(index))
                        setTransmit(index, True)

                    # Stop PTT
                    elif command == "stopTx":
                        logger.logVerbose("Stopping TX on radio index {}".format(index))
                        setTransmit(index, False)

                    # Channel Up
                    elif command == "chanUp":
                        changeChannel(index, False)

                    # Channel Down
                    elif command == "chanDn":
                        changeChannel(index, True)

                    # Buttons
                    elif command == "button":
                        # Get button
                        button = options

                        if button == "monitor":
                            toggleMonitor(index)

                        elif button == "nuisance":
                            nuisanceDelete(index)

                        elif button == "power":
                            togglePower(index)

                        elif button == "scan":
                            toggleScan(index)

                        elif button == "direct":
                            toggleDirect(index)

                #
                #   Audio Control Messages
                #

                elif key == "audioControl":
                    # Get params
                    params = cmdObject[key]
                    command = params["command"]
                    index = params["index"]

                    # Start audio command
                    if command == "startAudio":
                        logger.logInfo("Starting radio audio devices")
                        startSound()

                    # Mute commands
                    elif command == "mute":
                        toggleMute(index, True)

                    elif command == "unmute":
                        toggleMute(index, False)

                #
                #   WebRTC Messages
                #

                elif key == "webRtcOffer":
                    # Get params
                    offerObj = cmdObject[key]

                    # Create peer connection
                    await gotRtcOffer(offerObj)

                #
                #   Audio Data Messages
                #

                elif key == "audioData":
                    # Get params
                    params = cmdObject[key]
                    source = params["source"]
                    data = params["data"]
                    
                    if source == "mic":
                        handleMicData(data)

                #
                #   NACK if command wasn't handled above
                #

                else:
                    # Send NACK
                    await messageQueue.put('NACK')

        # Handle connection closing event (stop audio devices)
        except websockets.exceptions.ConnectionClosed:
            logger.logWarn("Client disconnected!")
            # stop sound devices and exit
            stopSound()
            break


async def producer_hander(websocket, path):
    """
    Websocket handler for sending data to client

    Args:
        websocket (websocket): socket object
        path (path): still not sure what this does
    """
    while True:
        try:
            # Wait for new data in queue
            message = await messageQueue.get()

            # send all radios
            if message == "allradios":
                logger.logInfo("sending radio list to {}".format(websocket.remote_address[0]))
                # Generate response JSON
                response = '{{ "radios": {{ "command": "list", "radioList": {} }} }}'.format(getAllRadiosStatusJson())
                # Send
                await websocket.send(response)

            # Send WebRTC SDP answer
            elif "webRtcAnswer" in message:
                logger.logInfo("sending WebRTC answer to {}".format(websocket.remote_address[0]))
                await websocket.send(message)
            
            # send status update for specific radio
            elif "status:" in message:
                # Get index
                index = int(message[7:])
                # Format JSON response
                response = '{{ "radio": {{ "index": {}, "status": {} }} }}'.format(index,getRadioStatusJson(index))
                # Send
                await websocket.send(response)
            
            # send speaker data from queue
            elif "speaker" in message:
                # Get samples
                speakerData = spkrSampleQueue.get_nowait()
                # Format response JSON
                response = '{{ "audioData": {{ "source": "speaker", "data": "{}" }} }}'.format(speakerData)
                # Send
                await websocket.send(response)
            
            # send NACK to unknown command
            elif "NACK" in message:
                logger.logWarn("invalid command received from {}".format(websocket.remote_address[0]))
                await websocket.send('{{"nack": {{ }} }}')
        
        except websockets.exceptions.ConnectionClosed:
            # The consumer handler will already cover this
            break


class httpServerHandler(http.server.SimpleHTTPRequestHandler):
    """
    Main handler for https server hosting the web gui
    """

    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory="console-client", **kwargs)

    def log_message(self, format, *args):
        """
        Surpress log messages for GET/POST requests
        """
        return

def startServer():
    """
    Start the websocket server and the http web gui server
    """

    global server
    global serverLoop

    logger.logInfo("Starting websocket server on address {}, port {}".format(address, serverport))
    # use ssl on the web socket
    ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    ssl_context.load_cert_chain("certs/localhost.crt", "certs/localhost.key")
    # create server object
    server = websockets.serve(websocketHandler, address, serverport, ssl=ssl_context)
    # start server async loop
    serverLoop = asyncio.get_event_loop()
    serverLoop.run_until_complete(server)

    # create TLS-secured HTTPServer
    logger.logInfo("Starting web GUI server on address {}, port {}".format(address, webguiport))
    httpServer = http.server.HTTPServer((address, webguiport), httpServerHandler)
    httpServer.socket = ssl.wrap_socket(httpServer.socket,
                                 server_side=True,
                                 certfile='certs/localhost.crt',
                                 keyfile='certs/localhost.key',
                                 ssl_version=ssl.PROTOCOL_TLS)
    # start thread for HTTPS server
    httpThread = threading.Thread(target=httpServer.serve_forever, daemon=True)
    httpThread.start()

"""-------------------------------------------------------------------------------
    Utility Functions
-------------------------------------------------------------------------------"""

BITMASK = 1
NIBMASK = 0xF
BYTEMASK = 0xFF

NIBSHIFT = 4
BYTESHIFT = 8

def getLsb(int_numb): ##returns position of the bit not the value of the bit
    if int_numb == 0 : return 0
    pos = 0
    while 1:
        if int_numb&(BITMASK<<pos)!=0:return pos+1
        pos+=1

def getMsb(int_numb):
    
    mask    = 0x8000000000000000
    chk_msk = 0xff00000000000000
    pos = 64
    while 1:
        
        if int_numb&chk_msk==0:
            chk_msk     >>= BYTESHIFT
            mask        >>= BYTESHIFT
            pos         -=  BYTESHIFT
        else:
            
            if int_numb&mask==0:
                mask >>= BITMASK
                pos   -= BITMASK
            else:
                break
        
        if pos == 0:break
    return pos

def bytemask(int_numb,mask=None):
    msk = 0
    i = 0
    while 1:
        
        msk += BYTEMASK<<(i*8)
            
        if int_numb&msk == int_numb:
            return msk
        i+=1

def twosCompliment(int_numb):
    if not int_numb: return ~int_numb
    
    lsb = getLsb(int_numb)
    msb = getMsb(int_numb)
    mask = ( 1<<lsb )-1
    mask ^= ( 1<<msb )-1
    return int_numb^mask

"""-------------------------------------------------------------------------------
    Main Runtime
-------------------------------------------------------------------------------"""

if __name__ == "__main__":
    
    try:

        # Start profiling
        #yappi.set_clock_type('cpu')
        #yappi.start(builtins=True)

        # Enable AIORTC debug
        logging.basicConfig(level=logging.ERROR)

        # add cli arguments
        addArguments()

        # parse the arguments
        parseArguments()

        # print OS
        #logger.logVerbose("Detected operating system: {}".format(osType))

        # Get sound devices
        getSoundDevices()

        # print loaded radios
        printRadios()

        # Start server
        startServer()

        # Connect to radios
        connectRadios()

        serverLoop.run_forever()

    except KeyboardInterrupt:
        logger.logWarn("Caught KeyboardInterrupt, shutting down")

        # Stop RTC
        serverLoop.run_until_complete(stopRtc())

        # Cleanly disconnect any connected radios
        for radio in config.RadioList:
            if radio.state != RadioState.Disconnected:
                radio.disconnect()
        logger.logVerbose("Disconnected from radios")

        # Stop PyAudio
        stopSound()
        pa.terminate()
        logger.logVerbose("Audio devices stopped")

        # Stop Loops
        serverLoop.stop()
        logger.logVerbose("Server loop stopped")

        # Stop profiling
        #stats = yappi.get_func_stats()
        #stats.save('callgrind.out', type='callgrind')

        # Exit without error
        exit(0)

    #except Exception as ex:
    #    logger.logError("Caught exception, exiting: {}".format(ex.args[0]))
    #    # Cleanly disconnect any connected radios
    #    for radio in config.RadioList:
    #        if radio.state != RadioState.Disconnected:
    #            radio.disconnect()
    #    # Exit with error code
    #    exit(1)