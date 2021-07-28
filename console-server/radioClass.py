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
    controlModes = ["None (RX only)",
                    "SB9600: XTL O-Head",
                    "SB9600: XTL W-Head",
                    "SB9600: Astro Spectra",
                    "SB9600: MCS2000",
                    "Soundcard: CM108/119 PTT",
                    "Soundcard: VOX PTT"]

    # Valid signalling modes for the radio
    signallingModes = ["None",
                       "MDC-1200",
                       "ANI",
                       "Single-tone",
                       "Two-tone"]

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

    # encode radio config into dict
    def encodeConfig(self):
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

    # decode dict into radio object
    def decodeConfig(radioDict):
        # create a new radio object from config data
        return Radio(radioDict['name'],
                     radioDict['desc'],
                     radioDict['ctrlMode'],
                     radioDict['ctrlPort'],
                     radioDict['txDevice'],
                     radioDict['rxDevice'],
                     radioDict['sigMode'],
                     radioDict['sigId'])