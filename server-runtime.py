# Base libraries
import sys
import os
import argparse
import platform
import threading
import time
import json
import queue
import audioop

# TCP Socket Server
import websockets
import websockets.exceptions
import asyncio

# HTTP server stuff
import socketserver
import http.server
import ssl

# WebRTC Stuff
from aiortc import MediaStreamTrack, RTCPeerConnection, RTCSessionDescription
from aiortc.contrib.media import MediaBlackhole, MediaPlayer, MediaRecorder
import uuid

# Sound stuff
import pyaudio
from mulaw import MuLaw

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

# Used for loading config
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

# Message queue for sending to client
messageQueue = asyncio.Queue()

# Audio globals
audioTransferSampleRate = 16000     # this is the samplerate used for audio transfer across the websocket

# Buffer durations in s (these must match the javascript variables)
spkrBufferDur = 0.2
micBufferDur = 0.2

spkrThread = None

# Client mic input vars
micSampleQueue = queue.Queue()

# Radio spkr output vars
spkrSampleQueue = queue.Queue()
spkrBufferString = ""
spkrBufferLength = 0

# Test input & output audio streams
micStream = None
spkrStream = None

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
                config.RadioList.append(Radio.decodeConfig(index, radioDict))

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
        radio.startAudio(pa, micSampleQueue, audioTransferSampleRate, micBufferDur, spkrBufferDur)

    # Start the speaker audio handler
    spkrThread = threading.Thread(target=handleSpkrData, daemon=True)
    spkrThread.start()

def stopSound():
    """
    Stops audio on each connected radio
    """

    for radio in config.RadioList:
        radio.stopAudio()

def handleMicData(dataString):
    """
    Route mic data from the client to the correct output devices

    Args:
        data (string): string of mu-law encoded mic data samples to process
    """
    # Split into a list of strings
    stringList = dataString.split(",")
    # Remove empty strings
    stringList[:] = [item for item in stringList if item]
    # Convert string list to floats
    uint8array = np.asarray(stringList, dtype=np.uint8)
    # Decode mu-law to float32
    floatArray = MuLaw.decode(uint8array)
    # Put to queue
    micSampleQueue.put_nowait(floatArray)
    # Print
    #logger.logInfo("Got {} mic samples from client".format(len(floatArray)))
    #logger.logInfo("Put mic samples in queue, new size {}".format(micSampleQueue.qsize()))


def handleSpkrData():
    """
    This is an infinite loop, to be run in a thread

    Get the speaker data from each connected radio, add it to the buffer string, and send & clear the buffer if it's big enough
    """
    global spkrBufferString
    global spkrBufferLength
    global spkrBufferDur

    while True:

        outputFloatArray = None
        gotSamples = False
        
        # Get samples from each radio and add to the output array
        for radio in config.RadioList:
            if radio.state == RadioState.Receiving and not radio.muted:
                try:
                    samples = radio.spkrQueue.get_nowait()
                    if not outputFloatArray:
                        gotSamples = True
                        outputFloatArray = samples
                    else:
                        outputFloatArray = np.add(outputFloatArray, samples)
                except queue.Empty:
                    #logger.logWarn("Radio {} queue empty".format(radio.name))
                    pass

        if gotSamples:
            # Convert to uint8 array of mu-law encoded samples
            muLawArray = MuLaw.encode(outputFloatArray)
            # this is allegedly a super-fast way to turn a float array into a comma-separated string
            dataString = ','.join([str(num) for num in muLawArray])
            # add this dataString to the buffer
            spkrBufferString += dataString
            spkrBufferLength += len(muLawArray)

            # Send the buffer string if it's big enough
            if spkrBufferLength >= spkrBufferDur * audioTransferSampleRate:
                # send this data string
                spkrSampleQueue.put_nowait(spkrBufferString)
                serverLoop.call_soon_threadsafe(messageQueue.put_nowait,"speaker")
                # clear the buffer
                spkrBufferString = ""
                spkrBufferLength = 0
        else:
            # give the CPU a break
            time.sleep(0.01)


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

            # Process the received command

            #
            #   Configuration Commands
            #

            if data == "?radios":
                # Send list of all radio statuses
                await messageQueue.put("allradios")

            #
            #   Radio Control Commands
            #

            elif data[0:9] == "!startTx:":
                # start transmit on specified radio
                index = int(data[9:])
                setTransmit(index, True)
            
            elif data[0:8] == "!stopTx:":
                # stop transmit on specified radio
                index = int(data[8:])
                setTransmit(index, False)

            elif data[0:8] == "!chanUp:":
                # change channel up on radio
                index = int(data[8:])
                changeChannel(index, False)

            elif data[0:8] == "!chanDn:":
                # change channel down
                index = int(data[8:])
                changeChannel(index, True)

            elif data[0:5] == "!mon:":
                index = int(data[5:])
                toggleMonitor(index)

            elif data[0:6] == "!nuis:":
                index = int(data[6:])
                nuisanceDelete(index)

            elif data[0:6] == "!lpwr:":
                index = int(data[6:])
                togglePower(index)

            elif data[0:6] == "!scan:":
                index = int(data[6:])
                toggleScan(index)

            elif data[0:5] == "!dir:":
                index = int(data[5:])
                toggleDirect(index)

            #
            #   Audio data messages
            #

            elif data == "!startAudio":
                logger.logInfo("Starting radio audio devices")
                startSound()

            elif data[0:9] == "micAudio:":
                micData = data[9:]
                handleMicData(micData)

            elif data[0:5] == "mute:":
                index = int(data[5:])
                toggleMute(index, True)

            elif data[0:7] == "unmute:":
                index = int(data[7:])
                toggleMute(index, False)

            #
            #   NACK if command wasn't handled above
            #

            else:
                # Send NACK
                await messageQueue.put("NACK")

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

            # get message type
            # send all radio parameters as a list
            if message == "allradios":
                logger.logInfo("sending radio list to {}".format(websocket.remote_address[0]))
                response = "radios:" + getAllRadiosStatusJson()
                await websocket.send(response)
            # send status update for specific radio
            elif "status:" in message:
                index = int(message[7:])
                #logger.logInfo("Sending status update for radio{}".format(index))
                await websocket.send("radio{}:".format(index) + getRadioStatusJson(index))
            # send speaker data from queue
            elif "speaker" in message:
                speakerData = spkrSampleQueue.get_nowait()
                await websocket.send("spkrAud:" + speakerData)
            # send NACK to unknown command
            elif "NACK" in message:
                logger.logWarn("invalid command received from {}".format(websocket.remote_address[0]))
                await websocket.send("NACK")
        
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
    # create server object
    server = websockets.serve(websocketHandler, address, serverport)
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

        # add cli arguments
        addArguments()

        # parse the arguments
        parseArguments()

        # print OS
        logger.logVerbose("Detected operating system: {}".format(osType))

        # Get sound devices
        getSoundDevices()

        # print loaded radios
        printRadios()

        # Start server
        startServer()

        # Connect to radios
        connectRadios()

        # Runtime loop
        asyncio.get_event_loop().run_forever()

    except KeyboardInterrupt:
        logger.logWarn("Caught KeyboardInterrupt, shutting down")
        # Cleanly disconnect any connected radios
        for radio in config.RadioList:
            if radio.state != RadioState.Disconnected:
                radio.disconnect()
        # Stop PyAudio
        stopSound()
        pa.terminate()

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