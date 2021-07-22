
class Radio():

    controlModes = ["Motorola Astro Spectra (SB9600)",
                    "Motorola MCS (SB9600)",
                    "Motorola XTL (SB9600)"]

    def __init__(self, name, desc, ctrlMode, ctrlDev, pttDev, txDev, rxDev, signalMode=None, signalId=None):
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
        self.name = name,
        self.desc = desc,
        self.ctrlMode = ctrlMode
        self.ctrlDev = ctrlDev
        self.pttDev = pttDev
        self.txDev = txDev
        self.rxDev = rxDev
        self.sigMode = signalMode,
        self.sigId = signalId