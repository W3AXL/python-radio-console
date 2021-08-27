
from interface.xtl import XTL

import queue

from radioState import RadioState

from logger import Logger

from mulaw import MuLaw

import samplerate as sr

import pyaudio

import numpy as np

import threading

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

    def __init__(self, index, name, desc=None, ctrlMode=None, ctrlPort=None, txDev=None, rxDev=None, signalMode=None, signalId=None):
        """Radio configuration object

        Args:
            index (int): index of the radio in the global radio list
            name (string): Radio name
            desc (string): Radio description
            ctrlMode (string): Radio control mode (SB9600, CAT, etc)
            ctrlDev (string): Device for radio control (serial, USB, IP, etc)
            txDev (string): Transmit audio device
            rxDev (string): Receieve audio device
            signalMode (string): Optional signalling mode
            signalId (any): Optional signalling ID
        """

        # Make sure control mode is valid
        if ctrlMode not in self.controlModes:
            raise ValueError("Invalid control mode specified: {}".format(ctrlMode))

        # Make sure signalling mode is valid
        if signalMode not in self.signallingModes:
            raise ValueError("Invalid signalling mode specified: {}".format(signalMode))

        # Save parameters
        self.index = index
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
        self.error = False
        self.scanning = False
        self.talkaround = False
        self.monitor = False
        self.lowpower = False
        self.selected = False

        # Radio interface class
        self.interface = None

        # Radio connection object
        self.connection = None

        # Set starting status to disconnected
        self.state = RadioState.Disconnected

        # Create logger
        self.logger = Logger()

        # Speaker & Mic Sample Queues
        self.spkrQueue = queue.Queue()

        # Volume set from client
        self.volume = 1.0

        # flag to keep transmitting until the mic queue is empty
        self.delayedTxStart = False
        self.delayedTxStop = False
        

    def connect(self, statusCallback, reset=True):
        """Connect to radio using specified communication scheme

        Args:
            statusCallback (function): callback to fire when radio status is updated, must take one integer argument
            reset (bool, optional): whether to reset the radio on connect or not. Defaults to True.
        """

        # XTL5000 O-head
        if self.ctrlMode == "SB9600-XTL-O":
            self.interface = XTL(self.index, self.ctrlPort, 'O5', statusCallback)
        
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
            self.delayedTxStart = True
        else:
            self.delayedTxStop = True

    def changeChannel(self, down):
        """
        Change channel up or down

        Args:
            down (bool, optional): Whether to go down.
        """
        self.interface.changeChannel(down)

    def toggleMonitor(self):
        """
        Toggles monitor state
        """
        self.interface.toggleMonitor()

    def nuisanceDelete(self):
        """
        Nuisance delete scan
        """
        self.interface.nuisanceDelete()

    def togglePower(self):
        """
        Power toggle button
        """
        self.interface.togglePower()

    def toggleScan(self):
        """
        Toggles state of scan
        """
        self.interface.toggleScan()

    def toggleDirect(self):
        """
        Toggle state of talkaround
        """
        self.interface.toggleDirect()

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
        self.talkaround = self.interface.talkaround
        self.monitor = self.interface.monitor
        self.lowpower = self.interface.lowpower

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
            "muted": self.interface.muted,
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
            "txDeviceIdx": self.txDev,
            "rxDeviceIdx": self.rxDev,
            "sigMode": self.sigMode,
            "sigId": self.sigId
        }
        return config

    def decodeConfig(index, radioDict):
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
                     radioDict['txDeviceIdx'],
                     radioDict['rxDeviceIdx'],
                     radioDict['sigMode'],
                     radioDict['sigId'])

    """-------------------------------------------------------------------------------
        Sound Device Functions
    -------------------------------------------------------------------------------"""

    def startAudio(self, pa, micQueue, transferSampleRate, micBufferDur, spkrBufferDur):
        """
        Start PyAudio devices for radio

        Args:
            pa (pyaudio): PyAudio instance
            micQueue (Queue): queue of float32 mic samples at transfer sample rate
            transferSamplerate (int): desired samplerate for samples sent to queue
            micBufferDur (float): size of mic buffer in s
            spkrBufferDur (float): size of speaker buffer in s
        """

        # Clear buffer
        self.clearSpkrQueue()

        # Set mic queue
        self.micQueue = micQueue

        # Get resampling ratios
        self.micResamplingRatio = 48000 / transferSampleRate
        self.spkrResamplingRatio = transferSampleRate / 48000

        # Create mic stream
        self.micStream = pa.open(
            format = pyaudio.paFloat32,
            channels = 1,
            rate = 48000,
            output = True,
            output_device_index = self.txDev,
            stream_callback = self.micCallback,
            frames_per_buffer = int(micBufferDur * 48000)
        )

        # Create speaker stream
        self.spkrStream = pa.open(
            format = pyaudio.paFloat32,
            channels = 1,
            rate = 48000,
            input = True,
            input_device_index = self.rxDev,
            stream_callback = self.spkrCallback,
            frames_per_buffer = int(spkrBufferDur * 48000)
        )

        # Start streams
        self.logger.logInfo("Starting audio streams for {}".format(self.name))
        self.micStream.start_stream()
        self.spkrStream.start_stream()

    def micCallback(self, in_data, frame_count, time_info, status):
        """
        Callback for client mic -> radio output device

        Args:
            in_data (array): not used for output device
            frame_count (int): number of frames to process
            time_info (time_info): not used
            status (pyaudio.status): pyaudio status

        Returns:
            constant: paContinue
        """
        # Check if we have a status
        if status:
            self.logger.logWarn("Got PyAudio status: {}".format(status))

        # Start with empty data
        data = np.zeros(frame_count).astype(np.float32)

        # Try to read from the queue, and ignore things if it's empty
        try:
            # Get the data (at transfer sample rate, as float32)
            floatArray = self.micQueue.get_nowait()
            # Start TX if we were delayed
            if self.delayedTxStart:
                #self.logger.logInfo("Delayed TX start")
                self.delayedTxStart = False
                self.interface.transmit(True)
            # Resample and set data
            data = sr.resample(floatArray, self.micResamplingRatio, 'sinc_fastest').astype(np.float32)
        except queue.Empty:
            # Stop TX if we were delayed
            if self.delayedTxStop:
                #self.logger.logInfo("Delayed TX stop")
                self.delayedTxStop = False
                self.interface.transmit(False)
            
        return (data, pyaudio.paContinue)

    def spkrCallback(self, in_data, frame_count, time_info, status):
        """
        Callback for radio spkr -> client input device
        fires whenever new speaker data is available (all the time)

        Args:
            in_data ([type]): [description]
            frame_count ([type]): [description]
            time_info ([type]): [description]
            status ([type]): [description]
        """

        # log status if we have one
        if status:
            self.logger.logWarn("Got PyAudio status: {}".format(status))

        # only send speaker data if we're receiving
        if self.state == RadioState.Receiving:
            # convert pyaudio samples to numpy float32 array and apply volume
            floatArray = np.frombuffer(in_data, dtype=np.float32) * self.volume
            # resample to desired sample rate
            resampled = sr.resample(floatArray, self.spkrResamplingRatio, 'sinc_fastest')
            # add samples to queue
            self.spkrQueue.put_nowait(resampled)

        return (None, pyaudio.paContinue)

    def stopAudio(self):
        """
        Stop audio devices for radio
        """
        self.micStream.stop_stream()
        self.spkrStream.stop_stream()
        self.clearMicQueue()
        self.clearSpkrQueue()

    def clearMicQueue(self):
        """
        Clear the mic queue cleanly
        """
        with self.micQueue.mutex:
            self.micQueue.queue.clear()
            self.micQueue.all_tasks_done.notify_all()
            self.micQueue.unfinished_tasks = 0

    def clearSpkrQueue(self):
        """
        Clear the speaker queue cleanly
        """
        with self.spkrQueue.mutex:
            self.spkrQueue.queue.clear()
            self.spkrQueue.all_tasks_done.notify_all()
            self.spkrQueue.unfinished_tasks = 0