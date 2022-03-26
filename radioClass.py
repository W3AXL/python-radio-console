
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

    def __init__(self, index, name, desc=None, ctrlMode=None, ctrlPort=None, txDev=None, rxDev=None, signalMode=None, signalId=None, logger=Logger()):
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
            "txDeviceIdx": self.txDev,
            "rxDeviceIdx": self.rxDev,
            "sigMode": self.sigMode,
            "sigId": self.sigId
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
                     radioDict['txDeviceIdx'],
                     radioDict['rxDeviceIdx'],
                     radioDict['sigMode'],
                     radioDict['sigId'],
                     logger)

    """-------------------------------------------------------------------------------
        Sound Device Functions
    -------------------------------------------------------------------------------"""

    def startAudio(self, pa, micQueue, audioSampleRate, micBufferFrames, spkrBufferFrames):
        """
        Start PyAudio devices for radio

        Args:
            pa (pyaudio): PyAudio instance
            micQueue (Queue): queue of float32 mic samples at transfer sample rate
            audioSampleRate (int): desired samplerate (must match what's configured on the client WebRTC connection)
            micBufferFrames (int): size of mic buffer
            spkrBufferFrames (int): size of speaker buffer
        """

        self.audioSampleRate = audioSampleRate

        # Clear buffer
        self.clearSpkrQueue()

        # Create queue for mic samples
        self.micQueue = queue.Queue()

        # Create mic stream
        self.micStream = pa.open(
            format = pyaudio.paFloat32,
            channels = 1,
            rate = audioSampleRate,
            output = True,
            output_device_index = self.txDev,
            stream_callback = self.micCallback,
            frames_per_buffer = micBufferFrames
        )

        # Create speaker stream
        self.spkrStream = pa.open(
            format = pyaudio.paInt16,
            channels = 1,
            rate = audioSampleRate,
            input = True,
            input_device_index = self.rxDev,
            stream_callback = self.spkrCallback,
            frames_per_buffer = spkrBufferFrames
        )

        # Start streams
        self.logger.logInfo("Starting audio streams for {}".format(self.name))
        self.logger.logVerbose("TX dev: {}, RX dev: {}".format(self.txDev, self.rxDev))
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
            self.logger.logWarn("{} got PyAudio status: {}".format(self.name, self.getPyaudioCallback(status)))
            self.logger.logWarn("Mic queue size {}".format(self.spkrQueue.qsize(),self.micQueue.qsize()))

        # Start with empty samples
        data = np.zeros(frame_count).astype(np.float32)

        # Debug - sine wave
        #data = (np.sin(2*np.pi*np.arange(frame_count)*440/self.audioSampleRate)).astype(np.float32)

        #print("Sent mic frame: ndim {}, shape {}, size {}, type {}, min {}, max {}".format(data.ndim, data.shape, data.size, data.dtype, np.min(data), np.max(data)))

        # only get samples if there's more than one chunk in the queue
        if self.micQueue.qsize() > 0:
            try:
                data = self.micQueue.get_nowait()
        
            except queue.Empty:
                # warn and send the zeroes
                self.logger.logWarn("Radio {} mic queue empty!".format(self.name))
            
        return (data, pyaudio.paContinue)

    def spkrCallback(self, in_data, frame_count, time_info, status):
        """
        Callback for radio spkr -> client device
        fires whenever new speaker data is available (all the time)

        Args:
            in_data ([type]): [description]
            frame_count ([type]): [description]
            time_info ([type]): [description]
            status ([type]): [description]
        """

        # log status if we have one
        if status and status not in [1,2,4,8]:
            self.logger.logWarn("{} got PyAudio status: {}".format(self.name, self.getPyaudioCallback(status)))

        # only send speaker data if we're receiving and not muted
        if self.state == RadioState.Receiving and not self.muted:
            # convert pyaudio samples to numpy float32 array and apply volume
            intArray = np.frombuffer(in_data, dtype=np.int16) * self.volume
            # add samples to queue
            self.spkrQueue.put_nowait(intArray)

        return (None, pyaudio.paContinue)

    def getPyaudioCallback(self, code):
        if code == 1:
            return "PyAudio Input Underflow ({})".format(pyaudio.paInputUnderflow)
        elif code == 2:
            return "PyAudio Input Underflow ({})".format(pyaudio.paInputOverflow)
        elif code == 4:
            return "PyAudio Input Underflow ({})".format(pyaudio.paOutputUnderflow)
        elif code == 8:
            return "PyAudio Input Underflow ({})".format(pyaudio.paOutputOverflow)
        else:
            return code

    def stopAudio(self):
        """
        Stop audio devices for radio
        """
        if self.micStream:
            self.micStream.stop_stream()
            self.micStream.close()
        if self.spkrStream:
            self.spkrStream.stop_stream()
            self.spkrStream.close()
        if self.micQueue:
            self.clearMicQueue()
        if self.spkrQueue:
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