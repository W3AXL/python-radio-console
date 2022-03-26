
from interface.xtl import XTL
from interface.xcmp import XPR

import queue

from radioState import RadioState

from logger import Logger

from mulaw import MuLaw

import samplerate as sr

import pyaudio

import numpy as np

import threading

import json

# Websocket stuff
import websockets
import websockets.exceptions
import asyncio
import ssl

# WebRTC Stuff
from aiortc import MediaStreamTrack, RTCPeerConnection, RTCSessionDescription, logging
from aiortc.contrib.media import MediaBlackhole, MediaPlayer, MediaRecorder, AudioFrame
import uuid


class Radio():
    """Radio Class for generic radio control
    """
    
    # Valid control modes for the radio
    controlModes = ["None",             # No PTT control
                    "XCMP-XPR",         # eXtended Control and Management Protocol for the XPR series radios
                    "SB9600-XTL-O",     # XTL O5
                    "SB9600-XTL-W",     # XTL W9
                    "SB9600-AS-W",      # Astro Spectra W9
                    "SB9600-MCS-3",     # MCS2000 model 3
                    "Soundcard-CM108",  # CM108 GPIO PTT
                    "Soundcard-VOX"]    # Radio-controlled VOX PTT

    # Valid signalling modes for the radio
    signallingModes = ["None",
                       "MDC",
                       "ANI",
                       "Singletone",
                       "QCII"]

    def __init__(self, name, certFile, keyFile, wsPort, wsAddr, desc=None, ctrlMode=None, ctrlPort=None, txDev=None, rxDev=None, logger=Logger()):
        """Radio configuration object

        Args:
            name (string): Radio name
            desc (string): Radio description
            certFile (string): SSL certificate file (for Websocket/WebRTC SSL)
            keyFile (string): SSL certificate key (for Websocket/WebRTC SSL)
            wsPort (int): Port for websocket connection
            wsAddr (string): Optional address on which to run websocket server
            ctrlMode (string): Radio control mode (SB9600, CAT, etc)
            ctrlDev (string): Device for radio control (serial, USB, IP, etc)
            txDev (int): Transmit audio device index
            rxDev (int): Receieve audio device index
            signalMode (string): Optional signalling mode
            signalId (any): Optional signalling ID
        """

        # Make sure control mode is valid
        if ctrlMode not in self.controlModes:
            raise ValueError("Invalid control mode specified: {}".format(ctrlMode))

        # Save parameters
        self.name = name
        self.desc = desc
        self.ctrlMode = ctrlMode
        self.ctrlPort = ctrlPort
        self.txDev = txDev
        self.rxDev = rxDev

        # Websocket parameters
        self.certFile = certFile
        self.keyFile = keyFile
        self.wsPort = wsPort
        self.wsAddr = wsAddr
        self.wsQueue = asyncio.Queue()

        # Init client-facing parameters
        self.zone = ""
        self.chan = ""
        self.lastid = ""
        self.error = False
        self.selected = False
        self.muted = False
        self.softkeys = ["","","","",""]
        self.softkeyStates = [False, False, False, False, False]

        # Radio interface class
        self.interface = None

        # Radio connection object
        self.connection = None

        # Set starting status to disconnected
        self.state = RadioState.Disconnected

        # Create logger
        self.logger = logger

        # Websocket Objects
        self.wsserver = None    # Websocket server
        self.wsloop = None      # Websocket handler loop

        # WebRTC objects
        self.rtcPeer = None     # WebRTC peer object
        self.micSink = None     # WebRTC output device
        self.spkrSource = None  # WebRTC input device

        # Speaker & Mic Sample Queues
        self.micQueue = None
        self.spkrQueue = queue.Queue()

        # Speaker & Mic Streams
        self.micStream = None
        self.spkrStream = None

        # Volume set from client
        self.volume = 1.0

        # Default samplerate
        self.audioSampleRate = 48000

        # flag to keep transmitting until the mic queue is empty
        self.delayedTxStart = False
        self.delayedTxStop = False        

    def connect(self, reset=True):
        """Connect to radio using specified communication scheme

        Args:
            reset (bool, optional): whether to reset the radio on connect or not. Defaults to True.
        """

        # XTL5000 O-head
        if self.ctrlMode == "SB9600-XTL-O":
            self.interface = XTL(self.ctrlPort, 'O5', self.updateStatus, self.logger)
        # XPR XCMP Control
        elif self.ctrlMode == "XCMP-XPR":
            self.interface = XPR(self.ctrlPort, self.updateStatus, self.logger)
        
        # Connect to radio (optional reset)
        self.interface.connect(reset=reset)

        # Start websocket listener
        self.startWebsocket()

    def disconnect(self):
        """
        Disconnect the radio
        """
        self.stopRtc()
        self.wsloop.stop()
        self.interface.disconnect()
        self.logger.logVerbose("Disconnected from radio {}".format(self.name))

    """-------------------------------------------------------------------------------
        Websocket server functions
    -------------------------------------------------------------------------------"""

    def startWebsocket(self):
        """
        Starts websocket server
        """

        if not self.wsAddr:
            self.wsAddr = 'localhost'
        
        self.logger.logInfo("Starting websocket server for radio {} on address {}, port {}".format(self.name, self.wsAddr, self.wsPort))

        # use ssl on the web socket
        ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
        ssl_context.load_cert_chain(self.certFile, self.keyFile)
        
        # create server object
        server = websockets.serve(self.websocketHandler, self.wsAddr, self.wsPort, ssl=ssl_context)
        
        # start server async loop
        self.wsloop = asyncio.get_event_loop()
        #serverLoop.run_until_complete(server)
        self.wsloop.run_forever()

    async def websocketHandler(self, websocket, path):
        """
        Sets up handlers for websocket
        """

        consumerTask = asyncio.ensure_future(self.consumerHandler(self, websocket, path))

        producerTask = asyncio.ensure_future(self.producer_hander(self, websocket, path))

        done, pending = await asyncio.wait(
            [consumerTask, producerTask],
            return_when=asyncio.FIRST_COMPLETED,
        )
        for task in pending:
            task.cancel()
        

    async def consumerHandler(self, websocket, path):
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
                    self.logger.logWarn("Invalid data recieved from client for radio {}, {}\nData: {}".format(self.name, e.args[0], data))
                    continue

                # Iterate through the received command keys (there should only ever be one, but it's possible to recieve multiple)

                for key in cmdObject.keys():

                    #
                    # Radio Query Command
                    #

                    if key == "radio" and cmdObject[key]["command"] == "query":
                        await self.wsQueue.put("radioInfo")

                    #
                    # Radio Control Commands
                    #

                    elif key == "radioControl":
                        # Get object inside
                        params = cmdObject[key]
                        command = params["command"]
                        options = params["options"]

                        # Start PTT
                        if command == "startTx":
                            self.logger.logVerbose("Starting TX on radio {}".format(self.name))
                            self.transmit(True)

                        # Stop PTT
                        elif command == "stopTx":
                            self.logger.logVerbose("Stopping TX on radio {}".format(self.name))
                            self.transmit(False)

                        # Channel Up
                        elif command == "chanUp":
                            self.changeChannel(False)

                        # Channel Down
                        elif command == "chanDn":
                            self.changeChannel(True)

                        # Buttons
                        elif command == "button":
                            # Get button
                            button = options

                            # Toggle a softkey
                            if "softkey" in button:
                                softkeyidx = int(button[7])
                                self.toggleSoftkey(softkeyidx)

                            # Left/right arrow keys
                            elif button == "left":
                                self.leftArrow()
                            elif button == "right":
                                self.rightArrow()

                    #
                    #   WebRTC Messages
                    #

                    elif key == "webRtcOffer":
                        # Get params
                        offerObj = cmdObject[key]

                        # Create peer connection
                        await self.gotRtcOffer(offerObj)

                    #
                    #   NACK if command wasn't handled above
                    #

                    else:
                        # Send NACK
                        await self.wsQueue.put('NACK')

            # Handle connection closing event (stop audio devices)
            except websockets.exceptions.ConnectionClosed:
                self.logger.logWarn("Client disconnected from radio {}!".format(self.name))
                # stop sound devices and exit
                self.disconnect()
                break


    async def producer_hander(self, websocket, path):
        """
        Websocket handler for sending data to client

        Args:
            websocket (websocket): socket object
            path (path): still not sure what this does
        """
        while True:
            try:
                # Wait for new data in queue
                message = await self.wsQueue.get()

                # send all radios
                if message == "radioInfo":
                    self.logger.logInfo("sending radio info for {} to {}".format(self.name, websocket.remote_address[0]))
                    # Generate response JSON
                    response = '{{ "radio": {{ "command": "list", "radioList": {} }} }}'.format(self.encodeClientStatus())
                    self.logger.logVerbose(json.dumps(response))
                    # Send
                    await websocket.send(response)

                # Send WebRTC SDP answer
                elif "webRtcAnswer" in message:
                    self.logger.logInfo("sending WebRTC answer for {} to {}".format(self.name, websocket.remote_address[0]))
                    await websocket.send(message)
                
                # send status update for specific radio
                elif "status" in message:
                    self.logger.logInof("sending radio status for {} to {}".format(self.name, websocket.remote_address[0]))
                    # Format JSON response
                    response = '{{ "radio": {{ "status": {} }} }}'.format(self.encodeClientStatus())
                    self.logger.logVerbose(json.dumps(response))
                    # Send
                    await websocket.send(response)
                
                # send NACK to unknown command
                elif "NACK" in message:
                    self.logger.logWarn("invalid command received from {}".format(websocket.remote_address[0]))
                    await websocket.send('{{"nack": {{ }} }}')
            
            except websockets.exceptions.ConnectionClosed:
                # The consumer handler will already cover this
                break

    """-------------------------------------------------------------------------------
        Radio command functions (shared by all interface types)
    -------------------------------------------------------------------------------"""

    def transmit(self, transmit):
        """
        Start or stop transmit, with a delay waiting for mic data

        Args:
            transmit (bool): transmit state
        """
        if transmit:
            self.interface.transmit(True)
        else:
            self.interface.transmit(False)

    def changeChannel(self, down):
        """
        Change channel up or down

        Args:
            down (bool, optional): Whether to go down.
        """
        self.interface.changeChannel(down)

    def toggleSoftkey(self, idx):
        """
        Toggles specified softkey

        Args:
            idx (int): 1-5, index of softkey
        """
        self.interface.toggleSoftkey(idx)

    def leftArrow(self):
        """
        Scrolls left through softkeys
        """
        self.interface.leftArrow()

    def rightArrow(self):
        """
        Scrolls right through softkeys
        """
        self.interface.rightArrow()

    def setMute(self, state):
        """
        Set status of radio mute and update
        """
        self.muted = state
        self.updateStatus()

    """-------------------------------------------------------------------------------
        Radio Status Functions
    -------------------------------------------------------------------------------"""

    def updateStatus(self):
        """
        Add a status update to the outgoing WS queue
        """
        self.wsloop.call_soon_threadsafe(self.wsQueue.put_nowait,"status")

    def getStatus(self):
        """
        Get status from the radio interface object
        """

        if self.state != self.interface.state:
            self.logger.logInfo("{} status now {} ({})".format(self.name, self.interface.state.name, self.interface.state.value))

        self.state = self.interface.state
        self.chan = self.interface.chanText
        self.zone = self.interface.zoneText
        self.softkeys = self.interface.softkeys
        self.softkeyStates = self.interface.softkeyStates

    def parseState(self):
        """Return current state of radio

        Returns:
            RadioState: current status of radio
            StateString: string representation of current status
        """

        if self.state.value < 10:
            stateString = self.state.name
            self.error = False
        else:
            self.error = True
            if self.state == RadioState.ConnectError:
                stateString = "Connection Error"
            elif self.state == RadioState.TransmitError:
                stateString = "Transmit Error"
            elif self.state == RadioState.ReceiveError:
                stateString = "Receive Error"
            else:
                stateString = "Unknown Error"

        return self.state, stateString

    def encodeClientStatus(self):
        """Encode radio status into a dict for sending to the client

        Returns:
            dict: Dict of radio status variables
        """

        # Get status from interface
        self.getStatus()

        # Parse radio status
        state, stateString = self.parseState()

        # Send error text if there's an error, or state text otherwise
        if state.value >= 10:
            stateText = "Error"
            errorText = stateString
        else:
            stateText = stateString
            errorText = ""

        # Encode status variables into a dict
        clientStatus = {
            "name": self.name,
            "zone": self.zone,
            "chan": self.chan,
            "lastid": self.lastid,
            "state": stateText,
            "muted": self.muted,
            "error": self.error,
            "errorText": errorText,
            "softkeys": self.softkeys,
            "softkeyStates": self.softkeyStates
        }
        
        # Return the dict
        return clientStatus

    def encodeConfig(self):
        """Encode radio parameters into config dict for JSON parsing

        Returns:
            dict: radio config parameters in dict form
        """
        # Create dict
        config = {
            "name": self.name,
            "desc": self.desc,
            "ctrlMode": self.ctrlMode,
            "ctrlPort": self.ctrlPort,
            "txDeviceIdx": self.txDev,
            "rxDeviceIdx": self.rxDev,
            "sigMode": self.sigMode,
            "sigId": self.sigId
        }
        return config

    def decodeConfig(radioDict, certFile, keyFile, wsAddr, logger=Logger()):
        """Decode a dict of config parameters into a new radio object

        Args:
            radioDict (dict): Dict of radio parameters

        Returns:
            Radio: a new Radio object
        """

        # create a new radio object from config data
        return Radio(
            name = radioDict['name'],
            certFile = certFile,
            keyFile = keyFile,
            wsPort = radioDict['port'],
            wsAddr = wsAddr,
            desc = radioDict['desc'],
            ctrlMode = radioDict['ctrlMode'],
            ctrlPort = radioDict['ctrlPort'],
            txDev = radioDict['txDev'],
            rxDev = radioDict['rxDev'],
            logger = logger
        )

    """-------------------------------------------------------------------------------
        WebRTC Configuration Functions
    -------------------------------------------------------------------------------"""

    async def gotRtcOffer(self, offerObj):
        """
        Handler for a received WebRTC offer

        Args:
            offerObj (dict): WebRTC offer
        """

        self.logger.logInfo("Got WebRTC offer for radio {}".format(self.name))
        self.logger.logVerbose("SDP: {}".format(offerObj['sdp']))
        
        # Create SDP offer and peer connection objects
        offer = RTCSessionDescription(sdp=offerObj["sdp"], type=offerObj["type"])
        self.rtcPeer = RTCPeerConnection()

        # Create UUID for peer
        pcUuid = "Peer({})".format(uuid.uuid4())
        self.logger.logVerbose("Creating peer connection {}".format(pcUuid))

        # Create the input & output devices
        self.micSink = MediaRecorder("out0", format="alsa")
        self.spkrSource = MediaPlayer("in0", format="alsa")

        # ICE connection state callback
        @self.rtcPeer.on("iceconnectionstatechange")
        async def onIceConnectionStateChange():
            self.logger.logVerbose("Ice connection state is now {}".format(self.rtcPeer.iceConnectionState))
            if self.rtcPeer.iceConnectionState == "failed":
                await self.rtcPeer.close()
                self.logger.logError("WebRTC peer connection {} failed".format(pcUuid))
        
        # Audio track callback when we get the mic track from the client
        @self.rtcPeer.on("track")
        async def onTrack(track):

            # make sure it's audio
            if track.kind == "audio":
                self.logger.logVerbose("Got audio track from peer {}".format(pcUuid))
                # Connect the incoming audio track to the mic sink
                self.micSink.addTrack(track)
                # Connect the speaker source back to the peer
                self.rtcPeer.addTrack(self.spkrSource.audio)
                # Log
                self.logger.logInfo("Connected RTC streams for radio {}".format(self.name))
            else:
                self.logger.logError("Got non-audio track from peer {} for radio {}, type = {}".format(pcUuid, self.name, track.kind))

            # Track ended handler
            @track.on("ended")
            async def onEnded():
                self.logger.logInfo("Mic track from {} for {} ended".format(pcUuid, self.name))

        @self.rtcPeer.on("datachannel")
        async def onDatachannel(channel):
            @channel.on("message")
            async def onMessage(message):
                if isinstance(message, str):
                    self.logger.logInfo("Got WebRTC data message for {}".format(self.name))
                    self.logger.logVerbose(message)

        await self.doRtcAnswer(offer)

        self.logger.logInfo("WebRTC connection complete for radio {} and peer {}".format(self.name, pcUuid))

    async def doRtcAnswer(self, offer):
    # Handle the received offer
        self.logger.logVerbose("Creating remote description from offer for radio {}".format(self.name))
        await self.rtcPeer.setRemoteDescription(offer)

        # Create answer
        self.logger.logVerbose("Creating WebRTC answer for radio {}".format(self.name))
        answer = await self.rtcPeer.createAnswer()

        # Set local description
        self.logger.logVerbose("setting local SDP for radio {}".format(self.name))
        await self.rtcPeer.setLocalDescription(answer)

        # Send answer
        self.logger.logVerbose("sending SDP answer for radio {}".format(self.name))
        message = '{{ "webRtcAnswer": {{ "type": "{}", "sdp": {} }} }}'.format(self.rtcPeer.localDescription.type, json.dumps(rtcPeer.localDescription.sdp))
        self.logger.logVerbose(message.replace("\\r\\n", "\r\n"))
        self.wsQueue.put_nowait(message)

    async def stopRtc(self):
        # Stop the peer if it's open
        self.logger.logVerbose("Stopping RTC connection for radio {}".format(self.name))
        if self.rtcPeer:
            await self.rtcPeer.close()