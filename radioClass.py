
from interface.xtl import XTL
from interface.xcmp import XPR

import queue

from radioState import RadioState

from logger import Logger

import numpy as np

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

    def __init__(self, index, name, desc=None, ctrlMode=None, ctrlPort=None, txDev=None, rxDev=None, logger=Logger()):
        """Radio configuration object

        Args:
            index (int): index of the radio in the global radio list
            name (string): Radio name
            desc (string): Radio description
            ctrlMode (string): Radio control mode (SB9600, CAT, etc)
            ctrlDev (string): Device for radio control (serial, USB, IP, etc)
            txDev (string): Transmit audio device
            rxDev (string): Receieve audio device
        """

        # Make sure control mode is valid
        if ctrlMode not in self.controlModes:
            raise ValueError("Invalid control mode specified: {}".format(ctrlMode))

        # Save parameters
        self.index = index
        self.name = name
        self.desc = desc
        self.ctrlMode = ctrlMode
        self.ctrlPort = ctrlPort
        self.txDev = txDev
        self.rxDev = rxDev

        # Init client-facing parameters
        self.zone = ""
        self.chan = ""
        self.lastid = ""
        self.error = False
        self.selected = False
        self.muted = False
        self.softkeys = ["","","","","",""]
        self.softkeyStates = [False, False, False, False, False, False]

        # Radio interface class
        self.interface = None

        # Radio connection object
        self.connection = None

        # Set starting status to disconnected
        self.state = RadioState.Disconnected

        # Create logger
        self.logger = logger

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
        

    def connect(self, statusCallback, reset=True):
        """Connect to radio using specified communication scheme

        Args:
            statusCallback (function): callback to fire when radio status is updated, must take one integer argument
            reset (bool, optional): whether to reset the radio on connect or not. Defaults to True.
        """

        # Store callback
        self.statusCallback = statusCallback

        # XTL5000 O-head
        if self.ctrlMode == "SB9600-XTL-O":
            self.interface = XTL(self.index, self.ctrlPort, 'O5', self.statusCallback, self.logger)
        # XPR XCMP Control
        elif self.ctrlMode == "XCMP-XPR":
            self.interface = XPR(self.index, self.ctrlPort, self.statusCallback, self.logger)
        
        # Connect to radio (optional reset)
        self.interface.connect(reset=reset)

    def disconnect(self):
        """
        Disconnect the radio
        """
        self.interface.disconnect()

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
            idx (int): 1-6, index of softkey
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
        self.statusCallback(self.index)

    """-------------------------------------------------------------------------------
        Radio Status Functions
    -------------------------------------------------------------------------------"""

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
            "txDev": self.txDev,
            "rxDev": self.rxDev
        }
        return config

    def decodeConfig(index, radioDict, logger=Logger()):
        """Decode a dict of config parameters into a new radio object

        Args:
            radioDict (dict): Dict of radio parameters

        Returns:
            Radio: a new Radio object
        """
        # create a new radio object from config data
        return Radio(index,
                     radioDict['name'],
                     radioDict['desc'],
                     radioDict['ctrlMode'],
                     radioDict['ctrlPort'],
                     radioDict['txDev'],
                     radioDict['rxDev'],
                     logger)