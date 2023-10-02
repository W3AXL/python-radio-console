
from typing import Dict
from interface.motorola import Motorola
from interface.xcmp import XPR

import queue

from radioState import RadioState

from logger import Logger

import numpy as np

from csv import DictReader

class Radio():
    """Radio Class for generic radio control
    """
    
    # Valid control modes for the radio
    controlModes = ["None",             # No PTT control
                    "XCMP-XPR",         # eXtended Control and Management Protocol for the XPR series radios
                    "Motorola-O5",      # XTL O5
                    "Motorola-W9",      # XTL/Astro W9
                    "Motorola-M3",    # MCS2000 model 3
                    "Soundcard-CM108",  # CM108 GPIO PTT
                    "Soundcard-VOX"]    # Radio-controlled VOX PTT

    def __init__(self, name, desc=None, ctrlMode=None, ctrlPort=None, txDev=None, rxDev=None, logger=Logger(), zoneLookup=None, chanLookup=None, btnBinding=None, softkeyList=None, useLedsForRx=False, rxOnly=False):
        """Radio configuration object

        Args:
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

        # Create logger
        self.logger = logger

        # Save parameters
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
        self.scanning = False
        self.priority = 0
        self.softkeys = ["","","","","",""]
        self.softkeyStates = [False, False, False, False, False, False]

        # Zone/Channel Lookup Dicts
        self.zoneLookup = zoneLookup
        self.chanLookup = chanLookup

        # W9/M3 softkey/btn lists
        self.btnBinding = btnBinding
        self.softkeyList = softkeyList

        # Use LEDs for RX states
        self.useLedsForRx = useLedsForRx

        # RX Only radio
        self.rxOnly = rxOnly

        if self.rxOnly:
            self.logger.logInfo("Radio configured for RX only, PTT will be disabled")

        # Radio interface class
        self.interface = None

        # Radio connection object
        self.connection = None

        # Set starting status to disconnected
        self.state = RadioState.Disconnected

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

        # Motorola O5 head
        if self.ctrlMode == "Motorola-O5":
            self.interface = Motorola(self.name, self.ctrlPort, 'O5', self.statusCallback, self.zoneLookup, self.chanLookup, self.logger, self.useLedsForRx)
        # Motorola W9 head
        elif self.ctrlMode == "Motorola-W9":
            self.interface = Motorola(self.name, self.ctrlPort, 'W9', self.statusCallback, self.zoneLookup, self.chanLookup, self.logger, self.btnBinding, self.softkeyList, self.useLedsForRx)
        # Motorola MCS2000 M3 head
        elif self.ctrlMode == "Motorola-M3":
            self.interface = Motorola(self.name, self.ctrlPort, "M3", self.statusCallback, self.zoneLookup, self.chanLookup, self.logger, self.btnBinding, self.softkeyList, self.useLedsForRx)
        # XPR XCMP Control
        elif self.ctrlMode == "XCMP-XPR":
            self.interface = XPR(self.name, self.ctrlPort, self.statusCallback, self.logger)
        # Fallback error
        else:
            raise Exception("Unknown control mode: {}".format(self.ctrlMode))
        
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
        # Don't do anything if RX only
        if self.rxOnly:
            return
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

    def pressSoftkey(self, idx):
        """
        Press a softkey (don't release it)

        Args:
            idx (int): 1-6, index of softkey
        """
        self.interface.pressSoftkey(idx)

    def releaseSoftkey(self, idx):
        """
        Release a softkey

        Args:
            idx (int): 1-6, index of softkey
        """
        self.interface.releaseSoftkey(idx)

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
        self.logger.logInfo("Muted radio {}".format(self.name))
        self.statusCallback()

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
        self.scanning = self.interface.scanning
        self.priority = self.interface.priority
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
            "scanning": self.scanning,
            "priority": self.priority,
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

    def decodeConfig(radioDict, logger=Logger()):
        """Decode a dict of config parameters into a new radio object

        Args:
            radioDict (dict): Dict of radio parameters

        Returns:
            Radio: a new Radio object
        """
        # These are optional, so default to None
        zoneLookup = None
        chanLookup = None
        btnBinding = None
        softkeyList = None
        useLedsForRx = False
        rxOnly = False

        if 'zoneLookup' in radioDict:
            zoneLookup = radioDict['zoneLookup']
        if 'chanLookup' in radioDict:
            chanLookup = radioDict['chanLookup']
        if 'chanLookupFile' in radioDict:
            with open(radioDict['chanLookupFile'], 'r') as f:
                chanDict = {}
                for line in f:
                    data = line.rstrip().split(',')
                    chanDict[data[0]] = data[1]
                print(chanDict)
                chanLookup = chanDict
        if 'btnBinding' in radioDict:
            btnBinding = radioDict['btnBinding']
        if 'softkeyList' in radioDict:
            softkeyList = radioDict['softkeyList']
        if 'useLedsForRx' in radioDict:
            useLedsForRx = radioDict['useLedsForRx']
        if 'rxOnly' in radioDict:
            rxOnly = radioDict['rxOnly']

        # create a new radio object from config data
        return Radio(radioDict['name'],
                     radioDict['desc'],
                     radioDict['ctrlMode'],
                     radioDict['ctrlPort'],
                     radioDict['txDev'],
                     radioDict['rxDev'],
                     logger,
                     zoneLookup,
                     chanLookup,
                     btnBinding,
                     softkeyList,
                     useLedsForRx,
                     rxOnly)