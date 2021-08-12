from enum import Enum
import json

class RadioStatus(Enum):
    """Radio status enum
    """
    # Standard connection states
    Disconnected = 0
    Idle = 0
    Receiving  = 1
    Transmitting = 2
    # Error states
    ConnectError = 10
    TransmitError = 11
    ReceiveError = 12
    UnknownError = 20

class Radio():
    """Radio Class for generic radio control
    """
    
    # Valid control modes for the radio
    controlModes = ["None",             # No PTT control
                    "SB9600-XTL-O",     # XTL O-head
                    "SB9600-XTL-W",     # XTL W-head
                    "SB9600-SPECTRA",   # Astro Spectra W-head
                    "SB9600-MCS",       # MCS2000
                    "Soundcard-CM108",  # CM108 GPIO PTT
                    "Soundcard-VOX"]    # Radio-controlled VOX PTT

    # Valid signalling modes for the radio
    signallingModes = ["None",
                       "MDC",
                       "ANI",
                       "Singletone",
                       "QCII"]

    # Init class
    def __init__(self, name, desc=None, ctrlMode=None, ctrlPort=None, txDev=None, rxDev=None, signalMode=None, signalId=None):
        """Radio configuration object

        Args:
            name (string): Radio name
            desc (string): Radio description
            ctrlMode (string): Radio control mode (SB9600, CAT, etc)
            ctrlDev (string): Device for radio control (serial, USB, IP, etc)
            txDev (string): Transmit audio device
            rxDev (string): Receieve audio device
            signalMode (string): Optional signalling mode
            signalId (any): Optional signalling ID
        """

        # Save parameters
        self.name = name
        self.desc = desc
        self.ctrlMode = ctrlMode
        self.ctrlPort = ctrlPort
        self.txDev = txDev
        self.rxDev = rxDev
        self.sigMode = signalMode
        self.sigId = signalId

        # Init client-facing parameters
        self.zone = ""
        self.chan = ""
        self.lastid = ""
        self.muted = False
        self.error = False
        self.scanning = False
        self.talkaround = False
        self.monitor = False
        self.lowpower = False

        # Set starting status to disconnected
        self.status = RadioStatus.Disconnected

    def getStatus(self):
        """Return current status of radio

        Returns:
            RadioStatus: current status of radio
            StatusString: string representation of current status
        """

        if self.status.value < 10:
            statusString = self.status.name
        else:
            if self.status == RadioStatus.ConnectError:
                statusString = "Connection Error"
            elif self.status == RadioStatus.TransmitError:
                statusString = "Transmit Error"
            elif self.status == RadioStatus.ReceiveError:
                statusString = "Receive Error"
            else:
                statusString = "Unknown Error"

        return self.status, statusString

    def encodeClientStatus(self):
        """Encode radio status into a dict for sending to the client

        Returns:
            dict: Dict of radio status variables
        """
        # Get overall radio status
        status, statusString = self.getStatus()
        if status >= 10:
            errorText = statusString
        else:
            errorText = ""

        # Encode status variables into a dict
        clientStatus = {
            "name": self.name,
            "zone": self.zone,
            "chan": self.chan,
            "lastid": self.lastid,
            "muted": self.muted,
            "error": self.error,
            "errorText": errorText,
            "scanning": self.scanning,
            "talkaround": self.talkaround,
            "monitor": self.monitor,
            "lowpower": self.lowpower
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
            "txDevice": self.txDev,
            "rxDevice": self.rxDev,
            "sigMode": self.sigMode,
            "sigId": self.sigId
        }
        return config

    def decodeConfig(radioDict):
        """Decode a dict of config parameters into a new radio object

        Args:
            radioDict (dict): Dict of radio parameters

        Returns:
            Radio: a new Radio object
        """
        # create a new radio object from config data
        return Radio(radioDict['name'],
                     radioDict['desc'],
                     radioDict['ctrlMode'],
                     radioDict['ctrlPort'],
                     radioDict['txDevice'],
                     radioDict['rxDevice'],
                     radioDict['sigMode'],
                     radioDict['sigId'])