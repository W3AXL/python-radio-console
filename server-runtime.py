# Base libraries
import sys
import os
import argparse
import platform
import threading
import time
import json
import queue

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

# Websocket server and event loop
server = None
serverLoop = None

# Message queue for sending to client
messageQueue = asyncio.Queue()

# Audio globals
audioSampleRate = None
audioBufferSize = 4   # hardcoded, needs to be the same as the buffer sizes in the javascript(s)

# Client mic input vars
micSampleQueue = queue.Queue()

# Radio spkr output vars
spkrSampleQueue = queue.Queue()
spkrBufferString = ""
spkrBufferSize = 0

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

def parseArguments():
    
    global verbose
    global address
    global serverport
    global webguiport
    global noreset

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

def setupSound():
    """
    Setup test sound device for mic loopback
    """
    global micStream

    # Clear buffer (with mutex)
    with micSampleQueue.mutex:
        logger.logInfo("clearing mic sample queue")
        micSampleQueue.queue.clear()

    logger.logInfo("Setting up test audio devices")
    # Create client mic stream
    micStream = pa.open(
        format=pyaudio.paFloat32,
        channels=1,
        rate=audioSampleRate,
        output=True,
        stream_callback=micCallback,
        frames_per_buffer=128 * audioBufferSize
    )
    # Create radio speaker stream
    spkrStream = pa.open(
        format=pyaudio.paFloat32,
        channels=1,
        rate=audioSampleRate,
        input=True,
        input_device_index=4,
        stream_callback=spkrCallback,
        frames_per_buffer=128 * audioBufferSize
    )
    # Start streams
    logger.logInfo("Starting audio streams")
    micStream.start_stream()
    spkrStream.start_stream()

def stopSound():
    """
    Cleans up audio buffer on client disconnect
    """

    logger.logInfo("Cleaning up test audio device buffer")

    # Clear buffer (with mutex)
    with micSampleQueue.mutex:
        logger.logInfo("clearing mic sample queue")
        micSampleQueue.queue.clear()

def micCallback(in_data, frame_count, time_info, status):
    """
    Callback for client mic -> radio output device

    Args:
        in_data (array): not used for output device
        frame_count (int): number of frames to process
        time_info (time_info): not used
        status (pyaudio.status): pyaudio status

    Returns:
        [type]: [description]
    """
    # Check if we have a status
    if status:
        logger.logWarn(status)
    # Only get samples if we have a buffer
    if micSampleQueue.qsize() > 2:
        #logger.logInfo("outputCallback(), Q size: {}".format(micSampleQueue.qsize()))
        data = micSampleQueue.get_nowait()
    else:
        data = np.zeros(frame_count)
    return (data, pyaudio.paContinue)

def spkrCallback(in_data, frame_count, time_info, status):
    """
    Callback for radio spkr -> client input device

    Args:
        in_data ([type]): [description]
        frame_count ([type]): [description]
        time_info ([type]): [description]
        status ([type]): [description]
    """
    # log status if we have one
    if status:
        logger.logWarn(status)
        
    # add audio to queue if radio is receiving
    if config.RadioList[0].state == RadioState.Receiving:
        handleSpkrData(in_data)

    return (None, pyaudio.paContinue)

def handleMicData(dataString):
    """
    Route mic data from the client to the correct output devices

    Args:
        data (string): string of mic data to process
    """
    # Split into a list of strings
    stringList = dataString.split(",")
    # Remove empty strings
    stringList[:] = [item for item in stringList if item]
    # Convert string list to floats
    floatArray = np.asarray(stringList, dtype=np.float32)
    micSampleQueue.put_nowait(floatArray)

def handleSpkrData(in_data):
    """
    Get the speaker data from the pyaudio callback, add it to the buffer string, and send & clear the buffer if it's big enough
    """
    global spkrBufferString
    global spkrBufferSize
    
    # convert from whatever the hell format pyaudio uses to a numpy float32 array
    data = np.fromstring(in_data, dtype=np.float32)
    # this is allegedly a super-fast way to turn a float array into a comma-separated string
    # we use 4 decimals of precision, just like the mic audio in microphone-processor.js
    dataString = ','.join(['%.4f' % num for num in data])
    # add this dataString to the buffer
    spkrBufferString += dataString
    spkrBufferSize += len(data)

    # Send the buffer string if it's big enough
    if spkrBufferSize >= 128 * audioBufferSize:
        # send this data string
        spkrSampleQueue.put_nowait(spkrBufferString)
        serverLoop.call_soon_threadsafe(messageQueue.put_nowait,"speaker")
        # clear the buffer
        spkrBufferString = ""
        spkrBufferSize = 0

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

            elif data[0:8] == "micRate:":
                # import globals
                global audioSampleRate
                # set globals
                audioSampleRate = int(data[8:])
                logger.logInfo("Got client audio samplerate: {}".format(audioSampleRate))
                # Setup mic input stream if not already configured
                if not micStream or not spkrStream:
                    logger.logInfo("Stream(s) not started, setting up")
                    setupSound()

            elif data[0:9] == "micAudio:":
                micData = data[9:]
                handleMicData(micData)

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
                logger.logInfo("Sending status update for radio{}".format(index))
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
    httpThread = threading.Thread(target=httpServer.serve_forever, daemon=True).start()

"""-------------------------------------------------------------------------------
    Main Runtime
-------------------------------------------------------------------------------"""

if __name__ == "__main__":

    try:

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