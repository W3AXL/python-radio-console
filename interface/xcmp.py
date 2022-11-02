import threading
from binascii import hexlify
from radioState import RadioState
from logger import Logger
import time
import queue
from trboxcmp.xcmp import Xcmp
#import logging

#see if the user defined a keys file, else go to sample file
try:
    from interface import xcmp_keys
except ImportError:
    templogger = Logger()
    templogger.logWarn("No XCMP key file found; defaulting to zero keys")
    from interface import xcmp_keys_sample as xcmp_keys

class XPR:

    def __init__(self, name, hostname, statusCallback, logger=Logger()):
        """Init Function

        Args:
            hostname (string): IP or DNS of the radio (usually 192.168.10.1 unless you changed it - this is set in CPS.
                Theoretically you can run this over BT or Wifi but I wouldn't trust it if you need a proper reliable setup.)
            statusCallback (function): callback function for status updates
        """

        """
        A quick note on these variables:
            keys (array): Keys to authenticate with XNL. It is an array structured as such:
                keys = [0x00000000, 0x00000000, 0x00000000, 0x00000000]
                These keys are the property of Motorola Solutions and will not be included with this program.
            delta (int): Delta for authentication with XNL.
                Again this is the property of Motorola Solutions and will not be included with this program.
            kid (int): Key ID used to identify the key when authenticating. If in doubt, use 1.
        """

        # Store objects
        # Port is always 8002
        self.device = Xcmp(xcmp_keys.keys, xcmp_keys.delta, xcmp_keys.kid, self.process, hostname, 8002)
        self.statusCallback = statusCallback
        #logging.basicConfig(format='%(asctime)s %(levelname)-8s %(message)s', level=logging.DEBUG, datefmt='%Y-%m-%d %H:%M:%S')

        """
        These variables are common to all radio interface classes and are 
        queried by the base RadioClass whenever statusCallback() is called
        """
        self.name = name
        self.state = RadioState.Disconnected
        self.zoneText = ""
        self.chanText = ""
        self.scanning = False
        self.priority = 0
        self.softkeys = ["","","","","","HOME"]
        self.softkeyStates = [False, False, False, False, False, False]

        """
        These variables are specific to this radio class.
        I'm not sure what we'd do with these, but might be good for future expansion. Also acts as a check to make sure XCMP is online.
        """
        self.serial = ""
        self.swver = ""
        self.modelno = ""
        self.battery = 0

        # Logger
        self.logger = logger
        self.sourceName = "XPR-XCMP"

    def connect(self, reset=True):
        """
        Connect and optionally reset the radio

        Args:
            reset (bool, optional): Whether to reset the radio. Defaults to True.
        """

        if (self.device.connect()):
            #yay! we connected
            self.state = RadioState.Idle
        self.updateStatus()

        #query the radio for basic info
        #these are quasi-async; so we won't get a reply here but in the callback
        self.device.getVersion()
        self.device.getModel()
        self.device.getSerial()

        #change the mic to the rear accy port
        self.device.selectMic(1)
        #this may fail if you are not authenticated on XCMP at a high enough level

        #drop reset on the floor for now, i don't think it's necessary in this implementation

    def disconnect(self):
        """
        Disconnect and close cleanly
        """
        # Stop any TX if it's happening
        self.transmit(False)
        # Close the socket and clean up
        self.device.close()
        # Set status to disconnected
        self.state = RadioState.Disconnected
        self.updateStatus()

    def process(self, msgIn):
        """
        Callback for when we receive an XCMP msg
        """

        self.logger.logVerbose("Got XCMP message from radio {}: {}".format(self.name, msgIn))

        if (msgIn['type'] == 2):    # call status info
            payload = msgIn['payload']
            self.lastid = payload['rid']
            self.logger.debug("Setting last ID to {}".format(payload['rid']))
        elif (msgIn['type'] == 6):    # display text
            payload = msgIn['payload']
            if (payload['lineNo'] == 3):     # generally we get our channel text on line 3, this is the case with my gen2 radios, have not tested gen1
                self.chanText = payload['content']
        elif (msgIn['type'] == 3):  # radio status
            payload = msgIn['payload']
            if (payload['respType'] == 'rid'):
                self.rid = payload['content']
            elif (payload['respType'] == 'serial'):
                self.serial = payload['content']
            elif (payload['respType'] == 'model'):
                self.modelno = payload['content']
        elif (msgIn['type'] == 5):  # battery level
            payload = msgIn['payload']
            self.battery = payload['battLevel']

        # push our status to the main thread
        self.updateStatus()


    def updateStatus(self):
        """
        Call the status callback
        """
        self.statusCallback()

    def transmit(self, transmit):
        """
        Start or stop PTT to the radio

        Args:
            transmit (bool): PTT state
        """
        #idk if this is actually needed?
        self.device.selectMic(1)
        if transmit:
            self.device.ptt(1)
        else:
            self.device.ptt(0)

    def changeChannel(self, down):
        """
        Change channel up or down, depending on direction

        Args:
            down (bool): Move channel down, otherwise channel moves up
        """
        if down:
            self.device.chDown()
        else:
            self.device.chUp()

    def setChannel(self, channel):
        """
        Set channel to specified channel in current zone without paging up/down

        Args:
            channel (int): Channel number to set radio to
        """
        self.device.setChannel(channel)

    def setZone(self, zone):
        """
        Set zone to specified zone without paging up/down

        Args:
            zone (int): Zone number to set radio to
        """
        self.device.setZone(zone)
    
    def toggleMonitor(self):
        """
        Monitor button
        """
        # Press nuis button
        self.pressButton(self.O5Address.button_map["btn_key_1"], 0.05) 

    def nuisanceDelete(self):
        """
        Nuisance delete button
        """
        # Press nuis button
        self.pressButton(self.O5Address.button_map["btn_key_2"], 0.05)

    def togglePower(self):
        """
        Power button
        """
        # Press nuis button
        self.pressButton(self.O5Address.button_map["btn_key_3"], 0.05)

    def toggleScan(self):
        """
        Change state of scan by sending softkey button to toggle
        """
        # Press scan button
        self.pressButton(self.O5Address.button_map['btn_key_4'], 0.05)

    def toggleDirect(self):
        """
        DIR button
        """
        # Press scan button
        self.pressButton(self.O5Address.button_map['btn_key_5'], 0.05)

    def pressButton(self, code, duration):
        """
        Press a button for a specified duration, holding down BUSY so we don't get any weirdness

        Args:
            code (byte): button opcode
            duration (float): duration to press in seconds
        """
        self.device.sendButton(code, 1)
        time.sleep(duration)
        self.device.sendButton(code, 0)

    def printMsg(self, source, msg):
        """Debug message printing

        Args:
            source (string): Message source
            msg (string): message
        """

        self.logger.logInfo("{: >10} >>: {}".format(source, msg))

    def printDbg(self, msg):
        """Debug message printing

        Args:
            source (string): Message source
            msg (string): message
        """

        self.logger.logInfo("{: >10} >>: {}".format(self.sourceName, msg))