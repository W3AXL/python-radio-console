# Base libraries
import sys
import os
import argparse
import platform
import threading
import time

# TCP Socket Server
import socketserver

# Colorized logs
from colorama import init, Fore, Back, Style

# Sound stuff
import sounddevice as sd

# Serial stuff
import serial
import serial.tools.list_ports

# Radio class
from radioClass import Radio, RadioStatus

# TCP handler class
from tcpServer import serverTCPHandler

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
port = None

# Sound device lists
inputs = []
outputs = []
hostapis = []

# Detect operating system
osType = platform.system()

"""-----------------------------------------
    Command Line Argument Functions
-----------------------------------------"""

def addArguments():
    """Add command line arguments
    """
    parser.add_argument("-a","--address", help="Server address to bind to")
    parser.add_argument("-c","--config", help="Config file to load", metavar="config.json")
    parser.add_argument("-ls","--list-sound", help="List available sound devices", action="store_true")
    parser.add_argument("-lp","--list-ports", help="List available com ports", action="store_true")
    parser.add_argument("-p","--port", help="Server port")
    parser.add_argument("-v","--verbose", help="Enable verbose logging", action="store_true")

def parseArguments():
    
    global verbose
    global address
    global port

    # Parse the args
    args = parser.parse_args()

    # Make sure we've got a valid set of them

    # Verbose logging
    if args.verbose:
        verbose = True
        logInfo("Verbose logging enabled")

    # List available serial devices
    if args.list_ports:
        logInfo("Listing available serial ports")

    # List available sound devices
    if args.list_sound:
        print()
        getSoundDevices()
        printSoundDevices()

    # Make sure a config file was specified
    if not args.config:
        logWarn("No config file specified, exiting")
        exit(0)
    else:
        loadConfig(args.config)

    # Make sure port and optionally an address were specified
    if not args.port:
        logError("No server port specified!")
        exit(1)
    else:
        port = int(args.port)
        if not args.address:
            address = "localhost"
        else:
            address = args.address

"""-----------------------------------------
    Config Parsing Functions
-----------------------------------------"""

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
            for radioDict in configDict["RadioList"]:
                config.RadioList.append(Radio.decodeConfig(radioDict))

            # Print on success
            logInfo("Sucessfully loaded config file {}".format(filename))

            # Return true
            return True
    except Exception as ex:
        logError("Error loading config file: {}".format(ex.args[0]))
        return False

def printRadios():
    logInfo("Loaded radios:")
    for idx, radio in enumerate(config.RadioList):
        print("      - radio{}: {}".format(idx, radio.name))
        print("                {} control ({})".format(radio.ctrlMode, radio.ctrlPort))
        print("                Tx Audio dev: {} ({})".format(radio.txDev, getDeviceName('output',radio.txDev)))
        print("                Rx Audio dev: {} ({})".format(radio.rxDev, getDeviceName('input',radio.rxDev)))

"""-----------------------------------------
    Sound Device Functions
-----------------------------------------"""

def getSoundDevices():
    """Get available system sound devices
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
        logVerbose("Windows OS detected, only looking for WASAPI devices (api index {})".format(wasapiIndex))

    # iterate through devices
    for device in devices:
        # Ignore non-WASAPI devices in windows
        if osType == "Windows" and device["hostapi"] != wasapiIndex:
            logVerbose("Skiping non-WASAPI device {}".format(device["name"]))
            continue
        # Add device to input or output (or both!) based on available channels
        if device['max_input_channels'] > 0:
            inputs.append(device)
        if device['max_output_channels'] > 0:
            outputs.append(device)

def printSoundDevices():
    """Print queried sound devices
    """

    # Print inputs first
    logInfo("Available input devices:")
    for idx, input in enumerate(inputs):
        name = input['name']
        hostapi = hostapis[input['hostapi']]['name']
        print("{}: {}, api: {}".format(idx, name, hostapi))
    # Line break
    print()
    # Print outputs
    logInfo("Available output devices")
    for idx, output in enumerate(outputs):
        name = output['name']
        hostapi = hostapis[output['hostapi']]['name']
        print("{}: {}, api: {}".format(idx, name, hostapi))
    # Line break
    print()

def getDeviceName(type, idx):
    """Return name of audio device specified by type and idnex

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

"""-----------------------------------------
    TCP Server Functions
-----------------------------------------"""

def startServer():

    logInfo("Starting server on address {}, port {}".format(address, port))
    # create server object
    server = socketserver.TCPServer((address, port), serverTCPHandler)
    # start server in thread
    threading.Thread(target=server.serve_forever, daemon=True).start()

"""-----------------------------------------
    Logging Print Functions
-----------------------------------------"""

def logVerbose(msg):
    if verbose:
        print(Fore.WHITE + Style.DIM + "INFO: " + msg + Style.RESET_ALL)

def logInfo(msg):
    print(Fore.WHITE + "INFO: " + msg + Style.RESET_ALL)

def logWarn(msg):
    print(Fore.YELLOW + "WARN: " + msg + Style.RESET_ALL)

def logError(msg):
    print(Fore.RED + "ERROR: " + msg + Style.RESET_ALL)

"""-----------------------------------------
    Main Runtime
-----------------------------------------"""

if __name__ == "__main__":
    # init colorized logs
    init()

    # add cli arguments
    addArguments()

    # parse the arguments
    parseArguments()

    # print OS
    logVerbose("Detected operating system: {}".format(osType))

    # Get sound devices
    getSoundDevices()

    # print loaded radios
    printRadios()

    # Start server
    startServer()

    # Runtime loop
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        logWarn("Caught KeyboardInterrupt, shutting down")
        exit(0)