from enum import Enum

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

    controlModes = ["Motorola Astro Spectra (SB9600)",
                    "Motorola MCS (SB9600)",
                    "Motorola XTL (SB9600)"]

    signallingModes = ["None",
                       "MDC-1200",
                       "ANI",
                       "Single-tone",
                       "Two-tone"]

    # Init class
    def __init__(self, name, desc=None, ctrlMode=None, ctrlPort=None, pttDev=None, txDev=None, rxDev=None, signalMode=None, signalId=None):
        """Radio configuration object

        Args:
            name (string): Radio name
            desc (string): Radio description
            ctrlMode (string): Radio control mode (SB9600, CAT, etc)
            ctrlDev (string): Device for radio control (serial, USB, IP, etc)
            pttDev (string): PTT device for keying radio
            txDev (string): Transmit audio device
            rxDev (string): Receieve audio device
            signalMode (string): Optional signalling mode
            signalId (any): Optional signalling ID
        """
        self.name = name
        self.desc = desc
        self.ctrlMode = ctrlMode
        self.ctrlPort = ctrlPort
        self.pttDev = pttDev
        self.txDev = txDev
        self.rxDev = rxDev
        self.sigMode = signalMode
        self.sigId = signalId

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

    # JSON encode
    def encode(self):
        return self.__dict__