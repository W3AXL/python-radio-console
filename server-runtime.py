# Base libraries
#from cmath import asin
#from datetime import datetime
#from inspect import trace
import sys
import glob
import os
import argparse
import platform
import threading
#import time
import json
#import queue
#import audioop
import ssl
#from typing_extensions import runtime

# TCP Socket Server
import websockets
import websockets.exceptions
import asyncio

# HTTP server stuff
#import socketserver
import http.server
import ssl

# WebRTC Stuff
from aiortc import MediaStreamTrack, RTCPeerConnection, RTCSessionDescription, logging
from aiortc.contrib.media import MediaBlackhole, MediaPlayer, MediaRecorder, MediaRelay, AudioFrame
import uuid

# Sound stuff
from av.audio.frame import AudioFrame

# Numpy (used for sound processing)
#import numpy as np

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

# Memory profiling
import tracemalloc

# Config class (loaded from JSON)
class Config():
    # Init
    def __init__(self):
        # List of radio configs
        self.RadioList = []

# Create new config
configFile = ""
config = Config()

# List of AIORTC recorders (so we can keep track of and close them during shutdown) and blackholes (so we can dump additional incoming tracks)
recorders = []
blackholes = []

# FFMPEG device format (alsa for linux, dshow for windows, loaded at runtime)
ffmpegFormat = None

# Flag that says when we got the first track from the client (which would be the mic track)
gotMicTrack = False

# Argument parser
parser = argparse.ArgumentParser()

# Global variables
verbose = False
address = None
serverport = None
webguiport = None
noreset = False
cpuProfiling = False
memProfiling = False

# SSL globals
certfile = 'certs/localhost.crt'
keyfile = 'certs/localhost.key'

# Websocket server and event loop
server = None
serverLoop = None

# WebRTC variables
rtcPeer = None

# Message queue for sending to client
messageQueue = asyncio.Queue()

# Audio globals
audioSampleRate = 48000  # this must match the WebRTC settings on the client

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
    parser.add_argument("-vv","--verbose2", help="Debug verbosity in logging", action="store_true")
    parser.add_argument("-wc","--webguicert", help="Web GUI certificate for TLS")
    parser.add_argument("-wp","--webguiport", help="Web GUI port")
    parser.add_argument("-cp", "--cpu-profiling", help="Enable yappi CPU profiling", action="store_true")
    parser.add_argument("-mp","--memory-profiling", help="Enable memory profiling", action="store_true")

def parseArguments():
    
    global verbose
    global address
    global serverport
    global webguiport
    global noreset
    global cpuProfiling
    global memProfiling

    # Parse the args
    args = parser.parse_args()

    # Make sure we've got a valid set of them

    # Verbose logging
    if args.verbose:
        logger.setVerbose(args.verbose)
        logger.logVerbose("Verbose logging enabled")

    if args.verbose2:
        logger.setVerbose(True)
        logger.setDebug(True)
        logger.logDebug("Debug logging enabled")

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
        cpuProfiling = True

    if args.memory_profiling:
        memProfiling = True

    # Make sure a config file was specified
    if not args.config:
        Logger.logWarn("No config file specified, exiting")
        exit(0)
    else:
        loadConfig(args.config)

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

    global certfile
    global keyfile
    global ffmpegFormat

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

            # Get globals
            if "Certfile" in configDict.keys() and "Keyfile" in configDict.keys():
                certfile = configDict["Certfile"]
                logger.logInfo("Using SSL cerfile: {}".format(certfile))
                keyfile = configDict["Keyfile"]
                logger.logInfo("Using SSL keyfile: {}".format(keyfile))

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
        print("                Tx Audio dev: {}".format(radio.txDev))
        print("                Rx Audio dev: {}".format(radio.rxDev))

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

def toggleSoftkey(index, softkeyidx):
    """
    Toggles softkey on radio

    Args:
        index (int): Index of radio in RadioList
        softkeyidx (int): Index of softkey (1-5)
    """
    config.RadioList[index].toggleSoftkey(softkeyidx)

def leftArrow(index):
    """
    Presses left arrow button (for softkey scrolling)

    Args:
        index (int): Index of radio in RadioList
    """
    config.RadioList[index].leftArrow()

def rightArrow(index):
    """
    Presses right arrow button (for softkey scrolling)

    Args:
        index (int): Index of radio in RadioList
    """
    config.RadioList[index].rightArrow()

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

async def gotRtcOffer(offerObj):
    """
    Called when we receive a WebRTC offer from the client

    Args:
        offerObj (dict): WebRTC SDP offer object
    """

    global rtcPeer

    logger.logInfo("Got WebRTC offer")
    logger.logVerbose("SDP: {}".format(offerObj['sdp']))

    # Start audio on radios
    #logger.logVerbose("Starting audio devices on radios")
    #startSound()
    
    # Create SDP offer and peer connection objects
    offer = RTCSessionDescription(sdp=offerObj["sdp"], type=offerObj["type"])
    rtcPeer = RTCPeerConnection()

    # Create UUID for peer
    pcUuid = "Peer({})".format(uuid.uuid4())
    logger.logVerbose("Creating peer connection {}".format(pcUuid))

    # Create speaker tracks for each radio
    for radio in config.RadioList:
        logger.logInfo("Creating RTC speaker track for radio {}, device {}".format(radio.name, radio.rxDev))
        player = MediaPlayer(radio.rxDev, format=ffmpegFormat)
        rtcPeer.addTrack(player.audio)

    # DEBUG: just do the first radio in the list
    #radio = config.RadioList[0]
    #logger.logInfo("Creating RTC speaker track for radio {}".format(radio.name))
    #player = MediaPlayer(radio.rxDev, format='alsa')
    #rtcPeer.addTrack(player.audio)

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
        global recorders
        global blackholes
        global gotMicTrack
        global ffmpegFormat

        logger.logVerbose("Got {} track from peer {}".format(track.kind, pcUuid))

        # make sure it's audio
        if track.kind != "audio":
            logger.logError("Got non-audio track from peer {}".format(pcUuid))
            return

        # if we already got a mic track, ignore this one
        if gotMicTrack:
            logger.logWarn("Ignoring additional track from peer since we already have the mic")
            blackhole = MediaBlackhole()
            blackhole.addTrack(track)
            await blackhole.start()
            blackholes.append(blackhole)
            logger.logVerbose("added additional track to blackhole list")
            return

        gotMicTrack = True

        # Create a relay for the incoming mic track
        micRelay = MediaRelay()
        # Add the mic track to each radio's tx device
        for radio in config.RadioList:
            logger.logInfo("Creating RTC mic track for radio {} using device {}, format {}".format(radio.name, radio.txDev, ffmpegFormat))
            recorder = MediaRecorder(radio.txDev, format=ffmpegFormat)
            recorder.addTrack(micRelay.subscribe(track))
            recorders.append(recorder)
            await recorder.start()

        # DEBUG: just do the first radio in the list
        #radio = config.RadioList[0]
        #logger.logInfo("Creating RTC mic track for radio {}".format(radio.name))
        #recorder = MediaRecorder(radio.txDev, format='alsa')
        #recorder.addTrack(track)
        #await recorder.start()

        # Track ended handler
        @track.on("ended")
        async def onEnded():
            global gotMicTrack
            global recorders
            global blackholes

            logger.logVerbose("Audio track from {} ended".format(pcUuid))
            logger.logInfo("Shutting down media devices")
            for recorder in recorders:
                await recorder.stop()
            for blackhole in blackholes:
                await blackhole.stop()
            # Reset mic track variable
            gotMicTrack = False

    await doRtcAnswer(offer)

    logger.logVerbose("done")

async def stopRtc():

    global gotMicTrack

    # Stop the peer if it's open
    logger.logInfo("Stopping RTC connection")
    if rtcPeer:
        await rtcPeer.close()
    # Reset mic track variable
    gotMicTrack = False

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
    logger.logVerbose(message.replace("\\r\\n", "\r\n"))
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

def printSoundDevices():
    """
    Print queried sound devices
    """

    # Print inputs first
    logger.logInfo("Available input devices:")
    
    print()

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

                        # Toggle a softkey
                        if "softkey" in button:
                            softkeyidx = int(button[7])
                            toggleSoftkey(index, softkeyidx)

                        # Left/right arrow keys
                        elif button == "left":
                            leftArrow(index)
                        elif button == "right":
                            rightArrow(index)

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
                        logger.logWarn("Deprecated command: startAudio")

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
                #   NACK if command wasn't handled above
                #

                else:
                    # Send NACK
                    await messageQueue.put('NACK')

        # Handle connection closing event (stop audio devices)
        except websockets.exceptions.ConnectionClosed:
            logger.logWarn("Client disconnected!")
            # stop sound devices and exit
            asyncio.ensure_future(stopRtc(),loop=serverLoop)
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
    global config

    logger.logInfo("Starting websocket server on address {}, port {}".format(address, serverport))
    # use ssl on the web socket
    ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    ssl_context.load_cert_chain(certfile, keyfile)
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
                                 certfile=certfile,
                                 keyfile=keyfile,
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

        # Enable AIORTC debug
        logging.basicConfig(level=logging.ERROR)

        # get & print OS
        if os.name == 'nt':
            ffmpegFormat = 'dshow'
        elif os.name == 'posix':
            ffmpegFormat = 'alsa'
        logger.logVerbose("Detected operating system: {}, using input mode {}".format(os.name, ffmpegFormat))

        # add cli arguments
        addArguments()

        # parse the arguments
        parseArguments()

        # Start CPU profiling
        #yappi.set_clock_type('cpu')
        #yappi.start(builtins=True)

        # Start Memory Profiling
        if memProfiling:
            logger.logInfo("Memory profiling enabled")
            tracemalloc.start(10)

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

        # Stop profiling
        #stats = yappi.get_func_stats()
        #timestamp = datetime.now().strftime("%Y-%m-%d_%H%M%S")
        #stats.save("callgrind_{}.out".format(timestamp), type='callgrind')

        # Stop memory profiling
        if memProfiling:
            snapshot = tracemalloc.take_snapshot()
            top_stats = snapshot.statistics('lineno')
            logger.logInfo("[ Top 10 Memory Users ")
            for stat in top_stats[:10]:
                logger.logInfo(stat)

        # Stop RTC
        asyncio.ensure_future(stopRtc(),loop=serverLoop)

        # Cleanly disconnect any connected radios
        for radio in config.RadioList:
            if radio.state != RadioState.Disconnected:
                radio.disconnect()
                logger.logVerbose("Radio {} disconnected".format(radio.name))

        # Shutdown all asyncio tasks
        try:
            loop = asyncio.get_running_loop()
            pending = asyncio.all_tasks()
            loop.run_until_complete(asyncio.gather(*pending))
            loop.stop()
            logger.logVerbose("Server loop stopped")
        except RuntimeError:
            logger.logVerbose("Server loop was not running")

        logger.logInfo("Server shutdown complete")

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