# Motorola Controller Class
# Uses SB9600
# 
# Original work copyright (C) 2014 Paul Banks (http://paulbanks.org)
#
# Modified for use with XTL series SB9600/SBEP commands by W3AXL
#

from time import sleep
from binascii import hexlify, unhexlify, b2a_uu
from bitstring import BitArray
from interface import sb9600
from logger import Logger
from radioState import RadioState
import threading
import time
import queue
import re
import math

# Addressable modules
MODULE_BCAST = 0
MODULE_RADIO = 1
MODULE_FRONTPANEL = 5

# SBEP module codes
sbep_modules = {
    'BCAST': 0x00,
    'RADIO': 0x01,
    'PANEL': 0x05
}

# Lamp mappings
lamps_map = {
    "L1": 0x0D, "L2RED": 0x0B, "L2GREEN": 0x0C, "L3": 0x01, "L4": 0x02, "L5":
    0x04, "L5B": 0x05, "L6": 0x10, "L7": 0x07, "L8": 0x11, "L9": 0x12, "L10":
    0x13, "L11": 0x0E, "L12": 0x0F, "L13": 0x14, "L14": 0x15, "L15": 0x16,
    "L16": 0x17, "L17": 0x18, "L18": 0x19,
}

# Lamp attributes
LAMP_OFF = 0
LAMP_ON = 1
LAMP_FLASH = 2

# Illumination addresses
ILLUM_MIC = 1  # TODO: A guess - I didn't have a mic attached to verify it!
ILLUM_DISPLAY = 2
ILLUM_BUTTONS = 3

# Control values
BUTTON_DOWN = 1
BUTTON_UP = 0


class Motorola:
    """Motorola Controller"""

    class O5:
        """
        SB9600/SBEP addresses for O5 control head
        """

        # O5 Button definitions
        button_map = {
            'ptt': 0x01,
            'hub': 0x06,
            'knob_vol': 0x02,
            'knob_chan': 0x04,
            'btn_light': 0x54,
            'btn_dp_lf': 0x80,
            'btn_home': 0x81,
            'btn_dp_rg': 0x82,
            'btn_key_1': 0x83,
            'btn_key_3': 0x84,
            'btn_key_5': 0x85,
            'btn_dp_up': 0x87,
            'btn_dp_dn': 0x88,
            'btn_key_2': 0x89,
            'btn_key_4': 0x8A,
            'btn_emgcy': 0x94
        }

        # O5 display icon definitions
        indicator_map = {
            'monitor': 0x01,
            'scan': 0x04,
            'scan_pri': 0x05,
            'direct': 0x07,
            'led_amber': 0x0f,
            'led_red': 0x10,
            'low_power': 0x56
        }

        # O5 display subdevices
        display_subdevs = {
            'text_zone': 0x00,
            'text_channel': 0x01,
            'text_softkeys': 0x02
        }

        # Strings to ignore when parsing channel text
        ignored_strings = [
            "SCAN ON",
            "SCAN OFF",
            "HIGH POWER",
            "LOW POWER",
            "MONITOR ON",
            "MONITOR OFF",
            "DIRECT MODE",
            "REPEATER MODE"
        ]

    class W9:
        """
        SB9600/SBEP addresses for W9 control head
        """

        # Button hex codes
        button_map = {
            'ptt': 0x01,
            'mode_down': 0x50,
            'mode_up': 0x51,
            'vol_down': 0x52,
            'vol_up': 0x53,
            'btn_top_1': 0x63,
            'btn_top_2': 0x64,
            'btn_top_3': 0x65,
            'btn_top_4': 0x66,
            'btn_top_5': 0x67,
            'btn_top_6': 0x68,
            'btn_kp_1': 0x31,
            'btn_kp_2': 0x32,
            'btn_kp_3': 0x33,
            'btn_kp_4': 0x34,
            'btn_kp_5': 0x35,
            'btn_kp_6': 0x36,
            'btn_kp_7': 0x37,
            'btn_kp_8': 0x38,
            'btn_kp_9': 0x39,
            'btn_kp_*': 0x3a,
            'btn_kp_0': 0x30,
            'btn_kp_#': 0x3b,
            'btn_home': 0x61,
            'btn_sel': 0x60,
            'btn_dim': 0x62
        }

        # Indicator hex codes
        indicator_map = {
            'ind_top_1': 0x7,
            'ind_top_2': 0x8,
            'ind_top_3': 0x9,
            'ind_top_4': 0xA,
            'ind_top_5': 0xB,
            'ind_top_6': 0xC,
            'ind_pri': 0xD,
            'ind_nonpri': 0xE,
            'ind_busy': 0xF,
            'ind_xmit': 0x10
        }

    # General state values shared between all head types
    class States:

        # SBEP indicator states
        indicator_states = {
            'off': 0x0,
            'on': 0x1,
            'flashing_1': 0x2,
            'flashing_2': 0x3
        }

    def __init__(self, name, port, head, statusCallback, zoneLookup=None, chanLookup=None, logger=Logger(), btnBinding=None, softkeyList=None):

        # Store status callback
        self.statusCallback = statusCallback

        # Logger
        self.logger = logger

        # Initialize SB9600 bus
        self.bus = sb9600.SB9600(port, logger=self.logger)
        self.head = head
        self.inSBEP = False
        self.logger = logger
        self.headType = head

        # lookup tables for extended zone/channel text mapping
        self.zoneLookup = zoneLookup
        self.chanLookup = chanLookup

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
        
        # Internal status flags for tracking softkeys
        self.monitor = False
        self.direct = False
        self.lowpower = False

        # Create buffer for single line display if W9
        if self.head == 'W9':
            self.display = '            '

        # Create arrays for button bindings and softkey list
        if self.head == 'W9':
            if not btnBinding or not softkeyList:
                raise ValueError("You must specify a btnBinding and softkeyList for a W9 head!")
            else:
                self.btnBinding = btnBinding
                self.softkeyList = softkeyList
                # These are used to change softkeys on radios without a true softkey system
                self.softkeyPage = 0
                self.maxKeyPage = math.floor(len(self.softkeyList) / 6)
                # Used to track softkey states when not on the same page
                self.indicatorStates = [False, False, False, False, False, False]

        # SB9600 retry counter (if sending a message fails)
        self.retries = 1

        # Flush the serial bus
        self.bus.ser.flush()

        # Reset bus waiting
        self.waiting = False

    def connect(self, reset=True):
        """
        Connect and optionally reset the radio

        Args:
            reset (bool, optional): Whether to reset the radio. Defaults to True.
        """
        # Create queue for serial messages
        self.rxMsgQueue = queue.Queue()
        self.txMsgQueue = queue.Queue()

        # Queue for delayed message sending (for long button presses, etc)
        # Each item has the format [calltime, function, *args]
        self.txDelayList = []

        # SBEP mode switch var
        self.inSBEP = False
        self.sbepSpeed = 0

        # Create listener thread and start it
        self.listenerThread = threading.Thread(target=self.listen, daemon=True)
        self.doListen = True
        self.listenerThread.start()

        # Create processor thread and start it
        #self.processorThread = threading.Thread(target=self.process, daemon=True)
        #self.doProcess = True
        self.statusTimer = time.time()
        self.newStatus = False
        #self.processorThread.start()

        # Reset the radio to init all the statuses
        if reset:
            self.bus.sb9600_reset()

        # If we're a W9, update the softkeys with the first five in the list
        if self.head == 'W9':
            self.softkeys = self.softkeyList[0:6]

        # Update radio state
        self.state = RadioState.Idle

    def disconnect(self):
        """
        Disconnect and close cleanly
        """
        # Stop any TX if it's happening
        self.transmit(False)
        # Stop listener thread
        self.doListen = False
        # Stop processor thread
        self.doProcess = False
        # Set status to disconnected
        self.state = RadioState.Disconnected
        self.updateStatus()

    def listen(self):
        """
        Main listener for SB9600 and SBEP messages
        """
        while self.doListen:
            # Run main receive loop
            self.receive()
            # Check if waiting
            if self.waiting:
                pass
            # If we're not waiting, send some data assuming we're idle
            elif not self.bus.isBusy():
                try:
                    # See if we have any messages waiting to be sent
                    txMsg = self.txMsgQueue.get_nowait()
                    # Send SB9600
                    if txMsg['type'] == 'SB9600':
                        sent = False
                        tries = 0
                        while (not sent) and (tries <= self.retries):
                            try:
                                tries += 1
                                self.bus.sb9600_send(txMsg['addr'], txMsg['prm1'], txMsg['prm2'], txMsg['func'])
                                sent = True
                            except RuntimeError as ex:
                                self.logger.logWarn("Message send failed, attempt {}/{}".format(tries, self.retries+1))

                except queue.Empty:
                    # Do nothing if queue empty
                    pass

            # Update status if needed
            if self.newStatus:
                self.newStatus = False
                self.updateStatus()

            # Check our pending delayed TX list
            if len(self.txDelayList) > 0:
                for idx, delayedFunc in enumerate(self.txDelayList):
                    if (time.time() * 1000) > delayedFunc[0]:
                        # call the function with arguments
                        delayedFunc[1](*delayedFunc[2])
                        # remove
                        self.txDelayList.pop(idx)

            # give the CPU a break
            time.sleep(0.01)

        # Print debug on thread close
        self.logger.logVerbose("SB9600 listener thread stopped for radio {}".format(self.name))

    def receive(self):
        """
        Main receiving function to be run in a loop
        """
        # We're actively receiving something
        if self.bus.isBusy() and self.waiting:
            pass
        # We just started receiving something
        elif self.bus.isBusy() and not self.waiting:
            self.waiting = True
        # We're done receiving
        elif self.waiting and not self.bus.isBusy():
            self.waiting = False
            # Read message
            msg = self.bus.read(self.bus.ser.in_waiting)
            # Process
            if len(msg) > 0:
                self.process(msg)

    def process(self, msg):
        """
        Main processor for SB9600 and SBEP messages
        """

        # Handle SBEP first
        if self.inSBEP:
            # reset
            self.inSBEP = False
            # Process
            processed = self.processSBEP(msg)
            self.logger.logVerbose("Exiting SBEP")
            # Check if SBEP decode worked
            if processed < 0:
                self.logger.logError("Failed to process SBEP message!")
            # See if we had any remaining SB9600 messages afterwards
            if len(msg) > processed:
                msg = msg[processed:]
                self.logger.logDebug("Processing remaining SB9600 data: {}".format(hexlify(msg, " ")))
                self.process(msg)

        # Handle SB9600
        else:
            self.logger.logDebug("Current msg buffer: {}".format(hexlify(msg, " ")))
            # Go through each message in the buffer
            while len(msg) >= 5:
                # Get the next message
                curMsg = msg[0:5]
                # Process it
                data = self.processSB9600(curMsg)
                # Trim that message from the array and repeat
                msg = msg[5:]
                # If we were commanded to go into SBEP and we have more data remaining, process it as SBEP now so we don't miss it
                if self.inSBEP and len(msg) > 5:
                    self.inSBEP = False
                    length = self.processSBEP(msg)
                    msg = msg[length:]

    def processSBEP(self, msg):
        """
        Main SBEP message processor

        Args:
            msg (byte[]): message array of bytes
        """

        totalBytes = 0
        origLength = len(msg)

        # Ignore the SBEP ack at the beginning of the byte string (if present)
        if msg[0] == 0x50:
            msg = msg[1:]
            totalBytes += 1

        # The following processing algorithm comes from Appendix I.5 of the SB9600 manual

        # Split out nibbles of first byte
        msn = (msg[0] >> 4) & 0xf
        lsn = (msg[0]) & 0xf

        # Starting values
        opcode = None
        length = None
        dataStart = 1   # with a normal opcode & length, data will be the second byte
        
        # Determine normal or extended opcode & normal or extended size
        if msn < 0xf:
            # Normal opcode
            opcode = msn
            if lsn < 0xf:
                # No extensions, size is lsn
                length = lsn
                self.logger.logDebug("Normal opcode SBEP message, normal size")
                self.logger.logDebug("    Opcode: {}, Length: {}".format(opcode, length))
            else:
                # Extended size, length is following two bytes
                length = int.from_bytes(msg[1:3], 'big')
                self.logger.logDebug("Normal opcode SBEP message, extended size")
                self.logger.logDebug("    Opcode: {}, Length: {}".format(opcode, length))
                dataStart = 3   # data starts after the 2-byte length
        else:
            # Extended opcode byte
            opcode = msg[1]
            if lsn < 0xf:
                # No size extensions, size is lsn
                length = lsn
                self.logger.logDebug("Extended opcode SBEP message, normal size")
                self.logger.logDebug("    Opcode: {}, Length: {}".format(opcode, length))
                dataStart = 2   # data starts after the extended opcode byte
            else:
                # Extended size, length is two bytes after extended opcode
                length = int.from_bytes(msg[2:4], 'big')
                self.logger.logDebug("Extended opcode SBEP message, extended size")
                self.logger.logDebug("    Opcode: {}, Length: {}".format(opcode, length))
                dataStart = 4   # data starts after the extended opcode byte and two size bytes

        # Extract message and SBEP data
        msg = msg[:dataStart + length]
        data = msg[dataStart:dataStart+length]
        self.logger.logDebug("SBEP msg:   {}".format(hexlify(msg, " ")))
        self.logger.logDebug("SBEP ASCII: {}".format(msg.decode('ascii','ignore')))

        # Checksum verification
        total = sum(msg[0:dataStart+length-1])
        checksum = 0xff - (total % 256)
        msgCheck = msg[-1]
        if msgCheck != checksum:
            self.logger.logError("SBEP checksum validation failed! Expected: {}, got: {}".format(hex(checksum),hex(msgCheck)))
            return -1

        # Used to feed back to processing routine if there was more than one message in a block of bytes
        totalBytes = dataStart + length

        # Ignore the last byte if it's outside our message and is the 0x50 ACK byte (basically makes sure we're not accidentally missing anything)
        if totalBytes < origLength and msg[-1] == 0x50:
            totalBytes += 1

        self.logger.logDebug("Read {} bytes".format(totalBytes))

        # Basic Opcodes
        if opcode == 0x1:
            
            # Display Opcode
            self.logger.logVerbose("SBEP Update Display")

            # Get message attributes
            row = data[0]       # cursor row
            col = data[1]       # cursor column
            chars = data[2]     # number of characters in message
            srow = data[3]      # starting row
            scol = data[4]      # starting column

            # Extract display characters
            text = data[5:5+chars].decode("ascii","ignore")
            self.logger.logDebug("text string from SBEP: len {}, text {}".format(chars, text))

            # W9 Display Update
            if self.head == 'W9':
                # Replace current channel text using column index
                self.logger.logDebug("Got W9 display update for row/col {}/{}".format(srow,scol))
                newDisplay = self.display[:scol] + text + self.display[scol+chars:]
                self.logger.logVerbose("New display text: {}".format(newDisplay))
                if newDisplay != self.display:
                    self.display = newDisplay
                    #self.zoneText = ""
                    self.newStatus = True
                    # Convert to zone/channel text if applicable
                    if self.zoneLookup:
                        for key in self.zoneLookup:
                            self.logger.logDebug("Checking if zone lookup key {} in new string {}".format(key, self.display))
                            if key.casefold() in self.display.casefold():
                                self.zoneText = self.zoneLookup[key]
                                self.logger.logVerbose("Got new zone text from lookup: {} = {}".format(key,self.zoneText))
                                # Strip the matched zone string from the channel display
                                newDisplay = re.sub(key, '', newDisplay, flags=re.IGNORECASE)
                                self.chanText = newDisplay
                    if self.chanLookup:
                        for key in self.chanLookup:
                            self.logger.logDebug("Checking if chan lookup key {} in new string {}".format(key, self.display))
                            if key.casefold() in self.display.casefold():
                                self.chanText = self.chanLookup[key]
                                self.logger.logVerbose("Got new chan text from lookup: {} = {}".format(key,self.chanText))
                return totalBytes

            elif self.head == 'O5':
                self.logger.logVerbose("Got O5 display update for srow/scol {}/{}".format(srow,scol))
                # Handle based on display subdevice
                if srow == self.O5.display_subdevs['text_zone']:
                    # Ignore zone text if it's the same as current, if it's in the ignored strings list, or if it's whitespace only
                    if text != self.zoneText and not any(s in text for s in self.O5.ignored_strings) and not text.isspace():
                        self.zoneText = text
                        self.newStatus = True
                        self.logger.logVerbose("Got new zone text: {}".format(text))
                        # Run lookup on text if applicable
                        if self.zoneLookup:
                            for key in self.zoneLookup:
                                self.logger.logDebug("Checking if zone lookup key {} in new string {}".format(key, self.display))
                                if key.casefold() in self.display.casefold():
                                    self.zoneText = self.zoneLookup[key]
                                    self.logger.logVerbose("Got new zone text from lookup: {} = {}".format(key,self.zoneText))
                    return totalBytes
                elif srow == self.O5.display_subdevs['text_channel']:
                    if text != self.chanText and not any(s in text for s in self.O5.ignored_strings):
                        self.chanText = text
                        self.newStatus = True
                        self.logger.logVerbose("Got new channel text: {}".format(text))
                        # Run lookup on text if applicable
                        if self.chanLookup:
                            for key in self.chanLookup:
                                self.logger.logDebug("Checking if chan lookup key {} in new string {}".format(key, self.display))
                                if key.casefold() in self.display.casefold():
                                    self.chanText = self.chanLookup[key]
                                    self.logger.logVerbose("Got new chan text from lookup: {} = {}".format(key,self.chanText))
                    return totalBytes
                elif srow == self.O5.display_subdevs['text_softkeys']:
                    # Get five softkey texts
                    self.softkeys = text.split('^')[1:6]
                    # Add home as the permanent sixth
                    self.softkeys.append("HOME")
                    self.updateSoftkeys()
                    self.newStatus = True
                    self.logger.logVerbose("Got new softkeys: {}".format(self.softkeys))
                    return totalBytes
        elif opcode == 0x2:
            # RF Hardware Test
            self.logger.logVerbose("SBEP RF Hardware Test")
        elif opcode == 0x3:
            # Virtual Source
            self.logger.logVerbose("SBEP Virtual Source")
        elif opcode == 0x5:
            # ACK
            self.logger.logVerbose("SBEP ACK")
        elif opcode == 0x6:
            # NACK
            self.logger.logVerbose("SBEP NACK")

        # Extended Opcodes (Broadcast)
        elif opcode == 0x10:
            # SBEP Reset Broadcast
            self.logger.logVerbose("SBEP Reset")
        elif opcode == 0x21:
            # Indicator Updates
            self.logger.logVerbose("SBEP Indicator Update")
            num = data[0]
            inds = data[1:1+num]
            states = data[1+num:1+num+num]
            for i in range(num):
                name = self.getIndicator(inds[i])
                state = self.getIndicatorState(states[i])
                self.logger.logVerbose("Indicator {} state {}".format(name,state))

                # If we're an O5, update the statuses based on the indicator states
                if self.head == "O5":
                    # Scan Icon
                    if name == "scan":
                        self.logger.logVerbose("Got new scanning state: {}".format(state))
                        if state == "on":
                            self.scanning = True
                        elif state == "off":
                            self.scanning = False
                        self.updateSoftkeys()
                        self.newStatus = True

                    # Scan Priority Dot
                    elif name == "scan_pri":
                        self.logger.logVerbose("Got new scan priority state: {}".format(state))
                        if state == "on":
                            self.logger.logVerbose("Channel priority 1 marker")
                            self.priority = 1
                        elif state == "flashing_1":
                            self.logger.logVerbose("Channel priority 2 marker")
                            self.priority = 2
                        elif state == "off":
                            self.logger.logVerbose("Channel priority marker cleared")
                            self.priority = 0
                        self.newStatus = True
                        self.updateSoftkeys()

                    # Low power L icon
                    elif name == 'low_power':
                        self.logger.logVerbose("Got new lowpower state: {}".format(state))
                        if state == "on":
                            self.lowpower = True
                        elif state == "off":
                            self.lowpower = False
                        self.updateSoftkeys()
                        self.newStatus = True

                    # Monitor icon
                    elif name == 'monitor':
                        self.logger.logVerbose("Got new monitor state: {}".format(state))
                        if state == "on":
                            self.monitor = True
                        elif state == "off":
                            self.monitor = False
                        self.updateSoftkeys()
                        self.newStatus = True

                    # Talkaround/direct icon
                    elif name == 'direct':
                        self.logger.logVerbose("Got new direct state: {}".format(state))
                        if state == "on":
                            self.direct = True
                        elif state == "off":
                            self.direct = False
                        self.updateSoftkeys()
                        self.newStatus = True

                # if we're a W9 and it's a top-row indicator, map it to the right softkey
                if self.head == 'W9' and 'ind_top_' in name:
                    indIdx = int(name[-1])-1
                    if state == "on":
                        self.indicatorStates[indIdx] = True
                    elif state == "off":
                        self.indicatorStates[indIdx] = False
                    # If it's specifically bound to the scan button, update scanning status
                    if self.btnBinding["btn_top_{}".format(indIdx+1)] == "SCAN":
                        if state == "on":
                            self.scanning = True
                        elif state == "off":
                            self.scanning = False
                    self.updateSoftkeys()
                    self.newStatus = True

                    
        
        # Fallback
        else:
            self.logger.logWarn("Unknown SBEP opcode: {}".format(opcode))

        return totalBytes

    def processSB9600(self, msg):
        """
        Main processor for SB9600 messages. Companion to above SBEP function
        """
        
        # Verify message is 5 bytes long
        if len(msg) > 5:
            raise ValueError("SB9600 message must be of length 5 (got: {})".format(len(msg)))

        self.logger.logDebug("SB9600 msg {}".format(hexlify(msg, " ")))

        # Extract parameters
        address = msg[0]
        data = msg[1:3]
        opcode = msg[3]
        check = msg[4]

        # Validate checksum (uses the message bytes minus the checksum)
        calcCheck = self.bus.sb9600_calcCRC(msg[0:4])
        if calcCheck != check:
            raise ValueError("SB9600 checksum failed! (got {}, expected {})".format(hex(calcCheck), hex(check)))

        

        if address == 0x00:
            """
            Broadcast Module Address
                handles transition to SBEP and overall channel states
            """

            # SBEP command
            if opcode == 0x06:
                # Put us into SBEP mode
                self.inSBEP = True

                # Get baudrate
                if data[0] == 0x12:
                    self.sbepSpeed = 9600
                else:
                    self.sbepSpeed = data[0]

                self.logger.logVerbose("Entering SBEP mode at {} baud".format(self.sbepSpeed))

                return

            # Set Button Command
            elif opcode == 0x0A:
                # Monitor mode
                if data[0] == 0x01:
                    if data[1] == 0x01:
                        if not self.monitor:
                            self.monitor = True
                            self.newStatus = True
                            self.logger.logInfo("Radio monitor on")
                            pass
                        return
                    else:
                        if self.monitor:
                            self.monitor = False
                            self.newStatus = True
                            self.logger.logInfo("Radio monitor off")
                            pass
                        return

                # TX mode
                elif data[0] == 0x03:
                    if data[1] == 0x01:
                        if self.state != RadioState.Transmitting:
                            self.logger.logInfo("Radio now transmitting")
                            self.state = RadioState.Transmitting
                            self.newStatus = True
                        return
                    else:
                        # Change to idle as long as we're not receiving
                        if (self.state != RadioState.Receiving) and (self.state != RadioState.Idle):
                            self.logger.logInfo("Radio now idle")
                            self.state = RadioState.Idle
                            self.newStatus = True
                        return

            # Power Up Status (PRUPST) reports the self test results
            elif opcode == 0x3b:
                group = data[1] >> 5        # group is highest 3 bits
                devAddr = data[1] & 0x1F    # device address is 5 lowest bits
                # If any bits are not 0, we have a test failure
                if data[0] != 0x00:
                    dataBits = BitArray(data[0]).bin[2:]
                    self.logger.logError("PRUPST detected errors from group {}, device {}: {}".format(hex(group),hex(devAddr), dataBits))
                else:
                    self.logger.logVerbose("Got normal selftest from group {}, device {}".format(hex(group), hex(devAddr)))
                return
            
            # Fallback for unknown message
            else:
                self.logger.logWarn("Unknown broadcast message opcode {}: params {}, {}".format(hex(opcode),hex(data[0]),hex(data[1])))
                return

        if address == 0x05:
            """
            Front Panel Module
                handles buttons and illumination. Icons and text are handled via SBEP
            """

            # button / knob
            if opcode == 0x57:
                # lookup button
                btn = self.getButton(data[0])
                if "knob" in btn:
                    # Knob CW/CCW
                    return
                else:
                    if data[1] == 0x01:
                        # Button pressed
                        self.logger.logInfo("Button {} pressed".format(btn))
                        return
                    else:
                        # Button released
                        self.logger.logInfo("Button {} released".format(btn))
                        return

            # backlighting / illumination
            elif opcode == 0x58:
                # display BL
                if data[0] == 0x02:
                    # Display backlight update
                    return
                elif data[0] == 0x03:
                    # Button backlight update
                    return

        if address == 0x01:
            """
            Radio Module
                handles audio and channel states
            """

            # Radio Ready (RADRDY) opcode
            if opcode == 0x15:
                self.logger.logDebug("Got Radio Ready (RADRDY) opcode")
                return

            # Radio Keyed (RADKEY) opcode
            elif opcode == 0x19:
                self.logger.logDebug("Got Radio Keyed (RADKEY) opcode")
                return

            # RX audio routing
            elif opcode == 0x1A:
                self.logger.logDebug("Got RX audio routing opcode")
                
            # audio mute opcode
            elif opcode == 0x1D:
                if data[1] == 0x01:
                    # Audio output unmuted
                    return
                elif data[1] == 0x00:
                    # Audio output muted
                    return

            # Squelch Detect (SQLDET) opcode, used for detecting receive state
            # least two bits of data[1] are all that we track
            elif opcode == 0x1e:
                # Channel idle
                if data[1] == 0x00:
                    # Change to idle as long as we're not transmitting and not already idle
                    if (self.state != RadioState.Idle) and (self.state != RadioState.Transmitting):
                        self.state = RadioState.Idle
                        self.newStatus = True
                    return
                # Channel RX
                if data[1] == 0x03:
                    if self.state !=RadioState.Receiving:
                        self.state = RadioState.Receiving
                        self.newStatus = True
                    return
                # This one can pop up when we're scanning and there's a nuisance deleted channel with activity
                # we ignore it for now
                if data[1] == 0x01:
                    return

                # Fallback
                else:
                    self.printMsg("Channel","Unknown state: {} {}".format(hex(data[0]),hex(data[1])))
                    return

            # Active Mode Update (ACTMDU) opcode
            elif opcode == 0x1f:
                optens = BitArray(data[0]).bin[2:]
                number = data[1]
                self.logger.logDebug("Got Active Mode Update (ACTMDU) opcode for mode number {} with option enables {}".format(number,optens))
                return

            # PL Detect (PLDECT) opcode
            # Only least two bits of data[1] matter here
            elif opcode == 0x23:
                if data[1] == 0x00:
                    self.logger.logDebug("No PL, channel unqualified")
                elif data[1] == 0x01:
                    self.logger.logDebug("No PL, channel qualified")
                elif data[1] == 0x02:
                    self.logger.logDebug("Valid PL, channel unqualified")
                elif data[1] == 0x03:
                    self.logger.logDebug("Valid PL, channel qualified")
                return

            # Display (DISPLY) opcode
            elif opcode == 0x3c:
                self.logger.logDebug("Got Display (DISPLY) update for field {}, data: {}".format(hex(data[0]),hex(data[1])))
                return

            # this one isn't in the SB9600 spec, must be something special for the XTLs
            elif opcode == 0x60:
                # Channel change ACK
                return

            elif (opcode == 0x1a) and (data[0], data[1] == 0x00):
                # Ignore this one, it gets spammed a bunch and I don't know what it is
                return

            # fallback
            else:
                self.logger.logWarn("Unknown message for radio module (0x01): opcode {}, params {} {}".format(hex(opcode),hex(data[0]),hex(data[1])))
                return

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
        if transmit:
            if self.head == "O5":
                self.sendButton(self.O5.button_map['ptt'], 0x01)
            elif self.head == "W9":
                self.sendButton(self.W9.button_map['ptt'], 0x01)
        else:
            if self.head == "O5":
                self.sendButton(self.O5.button_map['ptt'], 0x00)
            elif self.head == "W9":
                self.sendButton(self.W9.button_map['ptt'], 0x00)

    def changeChannel(self, down):
        """
        Change channel up or down, depending on direction

        Args:
            down (bool): Move channel down, otherwise channel moves up
        """
        if down:
            if self.head == 'O5':
                self.sendButton(self.O5.button_map['knob_chan'], 0xFF)
            elif self.head == 'W9':
                self.pressButton(self.W9.button_map['mode_down'])
        else:
            if self.head == 'O5':
                self.sendButton(self.O5.button_map['knob_chan'], 0x01)
            elif self.head == 'W9':
                self.pressButton(self.W9.button_map['mode_up'])

    def toggleSoftkey(self, idx):
        """
        Press softkey button

        Softkey 6 is hardcoded as HOME

        Args:
            idx (int): 1-5, softkey number
        """
        # O5 just sends a softkey press normally
        if self.head == 'O5':
            if idx == 6:
                # The O5 home button is a special case, we actually have to hold it for a short duration otherwise it doesn't register as a press
                self.holdButton(self.O5.button_map["btn_home"], 50)
            else:
                self.pressButton(self.O5.button_map["btn_key_{}".format(idx)])
        # W9 is more involved, we have to search the button mapping for the right key to press
        elif self.head == 'W9':
            softkey = self.softkeys[idx-1]
            for key, value in self.btnBinding.items():
                if value == softkey:
                    self.pressButton(self.W9.button_map[key])

    def leftArrow(self):
        """
        Presses left arrow button or toggles softkey list left
        """
        if self.head == 'O5':
            self.pressButton(self.O5.button_map["btn_dp_lf"])
        elif self.head == 'W9':
            if self.softkeyPage == 0:
                self.softkeyPage = self.maxKeyPage
            else:
                self.softkeyPage -= 1
            self.updateSoftkeyPage()

    def rightArrow(self):
        """
        Presses right arrow button or toggles softkey list right
        """
        if self.head == 'O5':
            self.pressButton(self.O5.button_map["btn_dp_lf"])
        elif self.head == 'W9':
            if self.softkeyPage == self.maxKeyPage:
                self.softkeyPage = 0
            else:
                self.softkeyPage += 1
            self.updateSoftkeyPage()

    def updateSoftkeyPage(self):
        """
        Used for radios without true softkeys only, changes the list of softkeys based on the current "page"
        """
        self.softkeys = self.softkeyList[6 * self.softkeyPage:6 * self.softkeyPage + 6]
        # Pad out the softkey list to always be 6 long
        while len(self.softkeys) < 6:
            self.softkeys += [""]
        self.logger.logVerbose("Softkey page {}/{}: {}".format(self.softkeyPage, self.maxKeyPage, self.softkeys))
        # Update the softkey states and send a new status
        self.updateSoftkeys()
        self.newStatus = True

    def getSbepModule(self, code):
        """Lookup SBEP module by hex code

        Args:
            code (byte): module code

        Returns:
            string: module name
        """

        for key, value in sb9600.sbep_modules.items():
            if code == value:
                return key
        return "{} (Unknown)".format(hex(code))
    
    def findSoftkey(self, name):
        """
        Finds the index (beginning at 0) of the softkey with the given name, or returns False if not present

        Args:
            name (str): softkey name
        """
        try:
            index = self.softkeys.index(name)
        except ValueError:
            index = None
        return index

    def updateSoftkeys(self):
        """
        Updates the softkey states based on the currently shown softkeys
        """
        # Clear all softkey states (softkey 6 is always false, HOME)
        self.softkeyStates = [False, False, False, False, False, False]
        # O5 just maps softkey to state based on softkey names
        if self.head == 'O5':
            # Scan
            idx = self.findSoftkey("SCAN")
            if idx != None:
                self.softkeyStates[idx] = self.scanning
            # Mon
            idx = self.findSoftkey("MON ")
            if idx != None:
                self.softkeyStates[idx] = self.monitor
            # Dir
            idx = self.findSoftkey("DIR ")
            if idx != None:
                self.softkeyStates[idx] = self.direct
            # Pwr
            idx = self.findSoftkey("PWR ")
            if idx != None:
                self.softkeyStates[idx] = self.lowpower
        # W9/M3 maps based on the btnMapping
        elif self.head == 'W9':
            # iterate through the 6 top indicators
            for i in range(6):
                btnName = "btn_top_{}".format(i+1)
                btnBind = self.btnBinding[btnName]
                idx = self.findSoftkey(btnBind)
                if idx != None:
                    self.logger.logDebug("Indicator/top button {} is bound to softkey {} at index {}".format(i, btnBind, idx))
                    self.softkeyStates[idx] = self.indicatorStates[i]
                else:
                    self.logger.logDebug("No softkey found for button mapping {}".format(btnBind))
    
    def getIndicator(self, code):
        """
        Lookup indicator code
        """
        if self.headType == 'O5':
            for key, value in self.O5.indicator_map.items():
                if code == value:
                    return key
            return "{} (Unknown)".format(hex(code))
        elif self.headType == 'W9':
            for key, value in self.W9.indicator_map.items():
                if code == value:
                    return key
            return "{} (Unknown)".format(hex(code))
        else:
            raise ValueError("Invalid head specified")

    def getIndicatorState(self, code):
        for key, value in self.States.indicator_states.items():
            if code == value:
                return key
        return "{} (Unknown)".format(hex(code))

    def getButton(self, code):
        """Lookup button by opcode

        Args:
            code (byte): button opcode

        Raises:
            ValueError: if control head invalid

        Returns:
            string: button name
        """
        if self.headType == 'O5':
            for key, value in self.O5.button_map.items():
                if code == value:
                    return key
            return "{} (Unknown)".format(hex(code))
        elif self.headType == 'W9':
            for key, value in self.W9.button_map.items():
                if code == value:
                    return key
            return "{} (Unknown)".format(hex(code))
        else:
            raise ValueError("Invalid head specified")

    def sendButton(self, code, value):
        """
        Add a button command to the tx message queue

        Args:
            code (byte): button code
            value (byte): value to send
        """
        msg = {
            "type": "SB9600",
            "addr": self.bus.sbep_modules['PANEL'],
            "prm1": code,
            "prm2": value,
            "func": 0x57
        }

        self.txMsgQueue.put_nowait(msg)

    def sendPRUPST(self, device, status=0x00):
        """
        Add a PRUPST (Power Up self test) message to the TX queue

        Args:
            device (byte): device address
            staus (byte): self test result (0x00 is all success)
        """
        msg = {
            "type": "SB9600",
            "addr": 0x00,
            "prm1": 0x00,
            "prm2": 0x05,
            "func": 0x3B
        }

        self.txMsgQueue.put_nowait(msg)

    def pressButton(self, code):
        """
        Press and release a button (this is a feature of SB9600, you can simulate a button press just by using code 0x2 instead of 0x1)

        Args:
            code (byte): button opcode
            duration (float): duration to press in seconds
        """
        self.sendButton(code, 0x02)

    def holdButton(self, code, duration):
        """
        Hold a button for a specified duration

        Args:
            code (byte): button opcode
            duration (float): duration to press in seconds
        """
        self.sendButton(code, 0x01)
        self.setTimeout(duration, self.sendButton, code, 0x00)

    def setTimeout(self, duration, func, *args):
        """
        Calls a function after a specified delay, non-blocking

        Args:
            duration (int): time to wait in ms
            func (function): function to call once time is up
        """
        execTime = (time.time() * 1000) + duration
        item = [execTime, func, [*args]]
        self.txDelayList.append(item)

    def getDisplaySubDev(self, code):
        """Lookup display subdevice by hex code

        Args:
            code (byte): module code

        Raises:
            ValueError: if control head invalid

        Returns:
            string: subdevice name
        """
        if self.headType == 'O5':
            for key, value in self.O5.display_subdevs.items():
                if code == value:
                    return key
            return "{} (Unknown)".format(hex(code))
        else:
            raise ValueError("Invalid head specified")

    def getDisplayIcon(self, code):
        """Lookup display icon by hex code

        Args:
            code (byte): icon code

        Raises:
            ValueError: if control head invalid

        Returns:
            string: icon name
        """
        if self.headType == 'O5':
            for key, value in self.O5.display_icons.items():
                if code == value:
                    return key
            return "{} (Unknown)".format(hex(code))
        else:
            raise ValueError("Invalid head specified")

    def printMsg(self, source, msg):
        """Debug message printing

        Args:
            source (string): Message source
            msg (string): message
        """

        self.logger.logInfo("{: >10} >>: {}".format(source, msg))
