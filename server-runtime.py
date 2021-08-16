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
import asyncio

# HTTP server stuff
import socketserver
import http.server

# Sound stuff
import sounddevice as sd

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

    # List available sound devices
    if args.list_sound:
        print()
        getSoundDevices()
        printSoundDevices()

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
            raise Exception("Specified file does not exist")

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
    except Exception as ex:
        logger.logError("Error loading config file: {}".format(ex.args[0]))
        return False

def printRadios():
    logger.logInfo("Loaded radios:")
    for idx, radio in enumerate(config.RadioList):
        print("      - radio{}: {}".format(idx, radio.name))
        print("                {} control ({})".format(radio.ctrlMode, radio.ctrlPort))
        print("                Tx Audio dev: {} ({})".format(radio.txDev, getDeviceName('output',radio.txDev)))
        print("                Rx Audio dev: {} ({})".format(radio.rxDev, getDeviceName('input',radio.rxDev)))

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

    # get available devices
    devices = sd.query_devices()
    # get available hostapis
    hostapis = sd.query_hostapis()

    # Get WASAPI index if we're on Windows
    if osType == "Windows":
        wasapiIndex = next((i for i, api in enumerate(hostapis) if "WASAPI" in api["name"]), None)
        logger.logVerbose("Windows OS detected, only looking for WASAPI devices (api index {})".format(wasapiIndex))

    # iterate through devices
    for device in devices:
        # Ignore non-WASAPI devices in windows
        if osType == "Windows" and device["hostapi"] != wasapiIndex:
            logger.logVerbose("Skiping non-WASAPI device {}".format(device["name"]))
            continue
        # Add device to input or output (or both!) based on available channels
        if device['max_input_channels'] > 0:
            inputs.append(device)
        if device['max_output_channels'] > 0:
            outputs.append(device)

def printSoundDevices():
    """
    Print queried sound devices
    """

    # Print inputs first
    logger.logInfo("Available input devices:")
    for idx, input in enumerate(inputs):
        name = input['name']
        hostapi = hostapis[input['hostapi']]['name']
        print("{}: {}, api: {}".format(idx, name, hostapi))
    # Line break
    print()
    # Print outputs
    logger.logInfo("Available output devices")
    for idx, output in enumerate(outputs):
        name = output['name']
        hostapi = hostapis[output['hostapi']]['name']
        print("{}: {}, api: {}".format(idx, name, hostapi))
    # Line break
    print()

def getDeviceName(type, idx):
    """
    Return name of audio device specified by type and idnex

    Args:
        type (string): "input" or "output"
        idx (int): index of device

    Raises:
        Exception if wrong type specified or index out of range

    Returns:
        string: name of device
    """
    
    if type == "input":
        if idx > (len(inputs)-1):
            raise Exception("Device index out of range")
        return inputs[idx]["name"]
    elif type == "output":
        if idx > (len(inputs)-1):
            raise Exception("Device index out of range")
        return outputs[idx]["name"]
    else:
        raise Exception("Invalid audio device type specified: {}".format(type))

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
        # Wait for data
        data = await websocket.recv()

        # Process the received command

        if data == "?radios":
            # Send list of all radio statuses
            await messageQueue.put("allradios")

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

        else:
            # Send NACK
            await messageQueue.put("NACK")


async def producer_hander(websocket, path):
    """
    Websocket handler for sending data to client

    Args:
        websocket (websocket): socket object
        path (path): still not sure what this does
    """
    while True:
        # Wait for new data in queue
        message = await messageQueue.get()

        # get message type
        if message == "allradios":
            logger.logInfo("sending radio list to {}".format(websocket.remote_address[0]))
            response = "radios:" + getAllRadiosStatusJson()
            await websocket.send(response)
        elif "status:" in message:
            index = int(message[7:])
            logger.logInfo("Sending status update for radio{}".format(index))
            await websocket.send("radio{}:".format(index) + getRadioStatusJson(index))
        elif "NACK" in message:
            logger.logWarn("invalid command received from {}".format(websocket.remote_address[0]))
            await websocket.send("NACK")


class httpServerHandler(http.server.SimpleHTTPRequestHandler):
    """
    Main handler for http server hosting the web gui
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

    logger.logInfo("Starting web GUI server on address {}, port {}".format(address, webguiport))
    # bind to socket server
    httpServer = socketserver.TCPServer((address, webguiport), httpServerHandler)
    # create thread
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
        # Exit without error
        exit(0)

    except Exception as ex:
        logger.logError("Caught exception, exiting: {}".format(ex.args[0]))
        # Cleanly disconnect any connected radios
        for radio in config.RadioList:
            if radio.state != RadioState.Disconnected:
                radio.disconnect()
        # Exit with error code
        exit(1)