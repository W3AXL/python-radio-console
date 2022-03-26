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
#from typing_extensions import runtime

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
import samplerate

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
    parser.add_argument("-v","--verbose", help="Enable verbose logging", action="store_true")
    parser.add_argument("-vv","--verbose2", help="Debug verbosity in logging", action="store_true")
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

    # Make sure a config file was specified
    if not args.config:
        Logger.logWarn("No config file specified, exiting")
        exit(0)
    else:
        loadConfig(args.config)

    # Default to localhost for address
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

    global certfile
    global keyfile

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
                config.RadioList.append(Radio.decodeConfig(radioDict, certfile, keyfile, address, logger))

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
        radio.connect(reset = not noreset)

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
    HTTP functions
-------------------------------------------------------------------------------"""

def startServer():
    """
    Start the HTTPS GUI server
    """

    global config

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

        # Get sound devices
        getSoundDevices()

        # print loaded radios
        printRadios()

        # Start server
        startServer()

        # Connect to radios
        connectRadios()

    except KeyboardInterrupt:
        logger.logWarn("Caught KeyboardInterrupt, shutting down")

        # Stop profiling
        #stats = yappi.get_func_stats()
        #stats.save('callgrind.out', type='callgrind')

        # Cleanly disconnect any connected radios
        for radio in config.RadioList:
            if radio.state != RadioState.Disconnected:
                radio.disconnect()

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