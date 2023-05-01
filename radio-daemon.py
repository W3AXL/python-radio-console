# Base libraries
#from cmath import asin
#from datetime import datetime
#from inspect import trace
import sys
import glob
import os
import argparse
import platform
import time
from datetime import datetime
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
#import yappi

# Memory profiling
#import tracemalloc
#import gc
#from pympler import summary, muppy

# Config class (loaded from JSON)
class Config():
    # Init
    def __init__(self):
        # List of radio configs
        self.Radio = None

# Create new config
configFile = ""
config = Config()

# AIORTC recorder & player (for mic & speaker tracks)
recorder = None
player = None

# Create a relay for the incoming mic track
micRelay = None

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
gcProfiling = False

# Muppy tracker
sumThread = None
sumStop = False
sumInterval = 300 # seconds

# SSL globals
certfile = 'certs/localhost.crt'
keyfile = 'certs/localhost.key'

# Asyncio routines and event loop
wsServer = None
eventLoop = None

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
    parser.add_argument("-cp", "--cpu-profiling", help="Enable yappi CPU profiling", action="store_true")
    parser.add_argument("-mp","--memory-profiling", help="Enable memory profiling", action="store_true")
    parser.add_argument("-gp","--garbage-profiling", help="Enable garbage collector debug", action="store_true")

    return

def parseArguments():
    
    global verbose
    global address
    global serverport
    global webguiport
    global noreset
    global cpuProfiling
    global memProfiling
    global gcProfiling

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

    if args.no_reset:
        noreset = True

    if args.cpu_profiling:
        cpuProfiling = True

    if args.memory_profiling:
        memProfiling = True

    if args.garbage_profiling:
        gcProfiling = True

    # Make sure a config file was specified
    if not args.config:
        Logger.logWarn("No config file specified, exiting")
        exit(0)
    else:
        loadConfig(args.config)

    return

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

            # Get and parse radio config
            config.Radio = Radio.decodeConfig(configDict["Radio"], logger)

            # Get globals
            if "Certfile" in configDict.keys() and "Keyfile" in configDict.keys():
                certfile = configDict["Certfile"]
                logger.logInfo("Using SSL cerfile: {}".format(certfile))
                keyfile = configDict["Keyfile"]
                logger.logInfo("Using SSL keyfile: {}".format(keyfile))

            # Print on success
            logger.logInfo("Sucessfully loaded config file {}".format(filename))
            logger.logVerbose(config)

            # Return true
            return True
    except ValueError as ex:
        logger.logError("Error loading config file: {}".format(ex.args[0]))
        return False

def printRadios():
    logger.logInfo("Loaded radio:")
    print("        {} control ({})".format(config.Radio.ctrlMode, config.Radio.ctrlPort))
    print("        Tx Audio dev: {}".format(config.Radio.txDev))
    print("        Rx Audio dev: {}".format(config.Radio.rxDev))

    return

"""-------------------------------------------------------------------------------
    Radio Functions
-------------------------------------------------------------------------------"""

def connectRadio():
    """
    Connect to the configured radio
    """
    # Log
    logger.logInfo("Connecting to radio {}".format(config.Radio.name))
    # Connect
    config.Radio.connect(radioStatusUpdate, reset = not noreset)

    return

def radioStatusUpdate():
    """
    Status callback the radio interface calls when it has a new status
    """

    # Add the message to the queue
    eventLoop.call_soon_threadsafe(messageQueue.put_nowait,"status")
    return


def setTransmit(transmit):
    """
    Set transmit state of the radio

    Args:
        transmit (bool): state of transmit
    """
    config.Radio.transmit(transmit)

    return


def changeChannel(down):
    """
    Changes the channel up or down on the radio

    Args:
        down ([type]): whether to go down or not
    """
    config.Radio.changeChannel(down)

    return

def toggleSoftkey(softkeyidx):
    """
    Toggles softkey on radio

    Args:
        softkeyidx (int): Index of softkey (1-5)
    """
    config.Radio.toggleSoftkey(softkeyidx)

    return

def pressSoftkey(softkeyidx):
    """
    Depresses softkey on radio

    Args:
        softkeyidx (int): Index of softkey (1-5)
    """
    config.Radio.pressSoftkey(softkeyidx)

    return

def releaseSoftkey(softkeyidx):
    """
    Releases softkey on radio

    Args:
        softkeyidx (int): Index of softkey (1-5)
    """
    config.Radio.releaseSoftkey(softkeyidx)

    return

def leftArrow():
    """
    Presses left arrow button (for softkey scrolling)
    """
    config.Radio.leftArrow()

    return

def rightArrow():
    """
    Presses right arrow button (for softkey scrolling)
    """
    config.Radio.rightArrow()
    
    return

def toggleMute(state):
    """
    Set state of mute for radio

    Args:
        state (bool): state of mute
    """
    config.Radio.setMute(state)

    return

def getRadioStatusJson():
    """
    Gets status of radio and returns a json string

    Returns:
        string: JSON of radio status
    """
    
    # Get the status of the specified radio
    status = config.Radio.encodeClientStatus()

    logger.logDebug(status)

    return json.dumps(status)
    
"""-------------------------------------------------------------------------------
    WebRTC Functions
-------------------------------------------------------------------------------"""     

async def gotRtcDescription(desc):
    """
    Called when we receive a WebRTC offer from the client

    Args:
        offerObj (dict): WebRTC description object
    """

    global rtcPeer
    global player
    global recorder
    global micRelay

    logger.logVerbose("Got WebRTC description")
    logger.logDebug(desc)

    # Create peer connection if it doesn't already exist
    if not rtcPeer:
        rtcPeer = RTCPeerConnection()

    # If offer, do offer-related things
    if desc["type"] == "offer":
    
        # Create SDP offer and peer connection objects
        offer = RTCSessionDescription(sdp=desc["sdp"], type=desc["type"])

        # Create speaker track
        if not player:
            logger.logInfo("Creating RTC speaker track for radio {}, device {}".format(config.Radio.name, config.Radio.rxDev))
            player = MediaPlayer(config.Radio.rxDev, format=ffmpegFormat)
            rtcPeer.addTrack(player.audio)
        else:
            logger.logVerbose("RTC speaker track already exists")

        # Create callbacks

        # ICE connection state callback
        @rtcPeer.on("iceconnectionstatechange")
        async def onIceConnectionStateChange():
            logger.logVerbose("Ice connection state is now {}".format(rtcPeer.iceConnectionState))
            if rtcPeer.iceConnectionState == "failed":
                await rtcPeer.close()
                logger.logError("WebRTC peer connection failed")
        
        # Audio track callback when we get the mic track from the client
        @rtcPeer.on("track")
        async def onTrack(track):

            global micRelay
            global recorder
            global ffmpegFormat

            logger.logVerbose("Got {} track from peer".format(track.kind))

            # make sure it's audio
            if track.kind != "audio":
                logger.logError("Got non-audio track from peer")
                return

            # Only open the mic recorder if it's not already open
            if not recorder:
                logger.logInfo("Opening TX audio device stream: {}".format(config.Radio.txDev))
                recorder = MediaRecorder(config.Radio.txDev, format=ffmpegFormat)

            # Create a mic relay
            if not micRelay:
                micRelay = MediaRelay()

            # Connect the mic track to the recorder via the relay
            recorder.addTrack(micRelay.subscribe(track))
            await recorder.start()

            # Track ended handler (don't really do anything here for now)
            @track.on("ended")
            async def onEnded():
                logger.logVerbose("Audio track from peer ended")

        # Send RTC resposne back to console

        await doRtcAnswer(offer)

        logger.logVerbose("done")

    return

async def stopRtc():
    global rtcPeer
    global player
    global recorder
    global micRelay
    # Stop the peer if it's open
    logger.logInfo("Stopping RTC connection")
    if rtcPeer:
        await rtcPeer.close()
        rtcPeer = None
    # Stop and clear recorder
    if recorder:
        logger.logVerbose("Stopping TX audio recorder")
        await recorder.stop()
        recorder = None
    # Stop and clear player
    if player:
        logger.logVerbose("Stopping RX audio player")
        player = None
    # Stop and clear the mic relay
    if micRelay:
        logger.logVerbose("Clearing mic relay")
        micRelay = None
    return

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
    logger.logDebug(message.replace("\\r\\n", "\r\n"))
    messageQueue.put_nowait(message)

    return

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

    return

def printSoundDevices():
    """
    Print queried sound devices
    """

    # Print inputs first
    logger.logInfo("Available input devices:")
    
    print()

    return

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

    return


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

    return
    

async def consumer_handler(websocket, path):
    """
    Websocket handler for data received from client

    Args:
        websocket (websocket): websocket object
        path (path): not sure what this does, we don't use it
    """

    
    try:
        # Old way - just a while True infinite loop
        #while True:

        # New way - proper asyncio consumer per documentation
        async for msg in websocket:
            logger.logDebug("Got data from websocket:")
            logger.logDebug(msg)

            obj = json.loads(msg)

            # Iterate through the received command keys (there should only ever be one, but it's possible to recieve multiple)

            for key in obj.keys():

                #
                # Radio Query Command
                #

                if key == "radio" and obj[key]["command"] == "query":
                    await messageQueue.put("status")

                #
                # Radio Control Commands
                #

                elif key == "radioControl":
                    # Get object inside
                    params = obj[key]
                    command = params["command"]
                    options = params["options"]

                    # Start PTT
                    if command == "startTx":
                        logger.logVerbose("Starting TX on radio")
                        setTransmit(True)

                    # Stop PTT
                    elif command == "stopTx":
                        logger.logVerbose("Stopping TX on radio")
                        setTransmit(False)

                    # Channel Up
                    elif command == "chanUp":
                        changeChannel(False)

                    # Channel Down
                    elif command == "chanDn":
                        changeChannel(True)

                    # Buttons
                    elif command == "buttonToggle":
                        # Get button
                        button = options

                        # Toggle a softkey
                        if "softkey" in button:
                            softkeyidx = int(button[7])
                            toggleSoftkey(softkeyidx)

                        # Left/right arrow keys
                        elif button == "left":
                            leftArrow()
                        elif button == "right":
                            rightArrow()

                    elif command == "buttonPress":
                        button = options
                        if "softkey" in button:
                            softkeyidx = int(button[7])
                            pressSoftkey(softkeyidx)

                    elif command == "buttonRelease":
                        button = options
                        if "softkey" in button:
                            softkeyidx = int(button[7])
                            releaseSoftkey(softkeyidx)

                    else:
                        logger.logWarn("Unknown radio control command {}".format(command))

                #
                #   Audio Control Messages
                #

                elif key == "audioControl":
                    # Get params
                    params = obj[key]
                    command = params["command"]

                    # Start audio command
                    if command == "startAudio":
                        logger.logWarn("Deprecated command: startAudio")

                    # Mute commands
                    elif command == "mute":
                        toggleMute(True)

                    elif command == "unmute":
                        toggleMute(False)

                #
                #   WebRTC Messages
                #

                elif key == "webRtc":
                    # Get params
                    rtcObj = obj[key]
                    logger.logVerbose("Got webRtc object from console")
                    logger.logDebug(rtcObj)
                    
                    # Determine if we have a candidate or a description
                    if 'desc' in rtcObj:
                        await gotRtcDescription(rtcObj['desc'])

                    if 'cand' in rtcObj:
                        logger.logWarn("Got WebRTC candidate, but we don't handle those yet")

                #
                #   NACK if command wasn't handled above
                #

                else:
                    logger.logError("Unknown command from console: {}".format(obj))
                    # Send NACK
                    await messageQueue.put('NACK')

    # Handle connection closing event (stop audio devices)
    except websockets.exceptions.ConnectionClosed:
        logger.logWarn("Client disconnected!")
        # stop sound devices and exit
        asyncio.ensure_future(stopRtc(),loop=eventLoop)
        return


async def producer_hander(websocket, path):
    """
    Websocket handler for sending data to client

    Args:
        websocket (websocket): socket object
        path (path): still not sure what this does
    """
    
    try:
        while True:
            # Wait for new data in queue
            message = await messageQueue.get()

            # Send WebRTC SDP answer
            if "webRtcAnswer" in message:
                logger.logInfo("sending WebRTC answer to {}".format(websocket.remote_address[0]))
                await websocket.send(message)
            
            # send status update for specific radio
            elif "status" in message:
                # Format JSON response
                response = '{{ "status": {} }}'.format(getRadioStatusJson())
                # Send
                await websocket.send(response)
            
            # send NACK to unknown command
            elif "NACK" in message:
                logger.logWarn("invalid command received from {}".format(websocket.remote_address[0]))
                await websocket.send('{{"nack": {{ }} }}')
        
    except websockets.exceptions.ConnectionClosed:
        # The consumer handler should already cover this
        return

def startWsServer():
    """
    Start the websocket server
    """

    global wsServer
    global eventLoop
    global config

    logger.logInfo("Starting websocket server on address {}, port {}".format(address, serverport))
    # use ssl on the web socket
    ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    ssl_context.load_cert_chain(certfile, keyfile)
    # create server object
    wsServer = websockets.serve(websocketHandler, address, serverport, ssl=ssl_context)
    # add websocket server to event loop
    eventLoop.run_until_complete(wsServer)
    #asyncio.run(wsServer)

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

def dump_garbage():
    """
    Show garbage collector statistics (for memory leak debugging)
    """
    # Force collection
    logger.logInfo("\nGARBAGE:")
    gc.collect()

    logger.logInfo("\nGARBAGE OBJECTS:")
    for x in gc.garbage:
        s = str(x)
        if len(s) > 80: s = s[:77]+'...'
        logger.logInfo("{}: {}".format(type(x),s))

    return

def getSummary():
    """
    Periodic pympler memory summary thread
    """
    global sumTracker
    global sumStop

    while not sumStop:
        logger.logInfo("Periodic memory summary")
        sum = summary.summarize(muppy.get_objects())
        summary.print_(sum)
        time.sleep(sumInterval)

"""-------------------------------------------------------------------------------
    Main Runtime
-------------------------------------------------------------------------------"""

if __name__ == "__main__":
    
    try:
        # Enable garbage collector
        #gc.enable()

        # Enable AIORTC debug
        logging.basicConfig(level=logging.ERROR)

        # Get asyncio event loop
        eventLoop = asyncio.get_event_loop()

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
        #if cpuProfiling:
        #    yappi.set_clock_type('cpu')
        #    yappi.start(builtins=True)

        # Start Memory Profiling
        #if memProfiling:
        #    logger.logInfo("Memory profiling enabled")
        #    tracemalloc.start(10)
            # Start tracking with muppy
            #logger.logInfo("Starting memory summary thread")
            #sumThread = threading.Thread(target=getSummary)
            #sumThread.start()

        # Start Garbage Collector Debug
        #if gcProfiling:
        #    logger.logInfo("Garbage collector profiling enabled")
        #    gc.set_debug(gc.DEBUG_LEAK)

        # Get sound devices
        getSoundDevices()

        # print loaded radios
        printRadios()

        # Start server
        startWsServer()

        # Connect to radio
        connectRadio()

        # Start asyncio event loop
        logger.logInfo("Starting main event loop")
        eventLoop.run_forever()

    except KeyboardInterrupt:
        logger.logWarn("Caught KeyboardInterrupt, shutting down")

        # Stop profiling
        if cpuProfiling:
            stats = yappi.get_func_stats()
            timestamp = datetime.now().strftime("%Y-%m-%d_%H%M%S")
            stats.save("callgrind_{}.out".format(timestamp), type='callgrind')

        # Stop memory profiling
        if memProfiling:
            snapshot = tracemalloc.take_snapshot()
            top_stats = snapshot.statistics('lineno')
            logger.logInfo("[ Top 10 Memory Users ")
            for stat in top_stats[:10]:
                logger.logInfo(stat)
            # Print final diff
            #sumStop = True
            logger.logInfo("Final memory summary")
            sum = summary.summarize(muppy.get_objects())
            summary.print_(sum)

        # Print Garbage Statistics
        if gcProfiling:
            logger.logInfo("Garbage collector statistics:")
            dump_garbage()

        # Stop RTC
        asyncio.ensure_future(stopRtc(),loop=eventLoop)

        # Disconnect radio if connected
        if config.Radio.state != RadioState.Disconnected:
            config.Radio.disconnect()
            logger.logVerbose("Radio {} disconnected".format(config.Radio.name))

        eventLoop.stop()
        logger.logVerbose("Event loop stopped")

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