from os import stat_result
from xml.sax import parseString
from interface import sb9600
import threading

from binascii import hexlify

from radioState import RadioState

from logger import Logger

import time

import queue

class XTL:

    class O5Address:
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
        display_icons = {
            'monitor': 0x01,
            'scan': 0x04,
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

    class W9Address:
        """
        SB9600/SBEP addresses for W9 control head
        """

        # Button hex codes
        button_map = {
            'ptt': 0x01,
            'mode_down': 0x50,
            'mode_up': 0x51,
            'vol_down': 0x52,
            'vol_up': 0x531
        }

    def __init__(self, index, comPort, headType, statusCallback, logger=Logger()):
        """Init Function

        Args:
            comPort (string): serial port for communication with RIB
            headType (string): control head type ('O5' only for now)
            statusCallback (function): callback function for status updates
        """

        # Store config variables
        self.comPort = comPort
        self.headType = headType
        self.statusCallback = statusCallback

        # Listener thread variables
        self.doListen = False

        """
        These variables are common to all radio interface classes and are 
        queried by the base RadioClass whenever statusCallback() is called
        """
        self.index = index
        self.state = RadioState.Disconnected
        self.zoneText = ""
        self.chanText = ""
        self.softkeys = ["","","","",""]
        self.softkeyStates = [False, False, False, False, False]
        
        # Internal status flags for tracking softkeys
        self.scanning = False
        self.monitor = False
        self.direct = False
        self.lowpower = False

        # SB9600 retry counter (if sending a message fails)
        self.retries = 1

        # Logger
        self.logger = logger


    def connect(self, reset=True):
        """
        Connect and optionally reset the radio

        Args:
            reset (bool, optional): Whether to reset the radio. Defaults to True.
        """

        # Create and flush serial bus
        self.bus = sb9600.Serial(self.comPort)
        self.bus.ser.flush()

        # Create queue for serial messages
        self.rxMsgQueue = queue.Queue()
        self.txMsgQueue = queue.Queue()

        # SBEP mode switch var
        self.inSBEP = False
        self.sbepSpeed = 0

        # Create listener thread and start it
        self.listenerThread = threading.Thread(target=self.listen, daemon=True)
        self.doListen = True
        self.listenerThread.start()

        # Create processor thread and start it
        self.processorThread = threading.Thread(target=self.process, daemon=True)
        self.doProcess = True
        self.statusTimer = time.time()
        self.newStatus = False
        self.processorThread.start()

        # Reset the radio to init all the statuses
        if reset:
            self.bus.sb9600_reset()

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
            # get a message if there is one
            if self.bus.ser.in_waiting > 0:
                while self.bus.isBusy():
                    pass
                # Read rest of message
                rxMsg = self.bus.read(self.bus.ser.in_waiting)
                # Put into queue
                self.rxMsgQueue.put_nowait(rxMsg)
                #self.logger.logVerbose("Got msg {}".format(hexlify(rxMsg," ")))

            # check for outgoing messages
            try:
                # get the message
                txMsg = self.txMsgQueue.get_nowait()
                # figure out what to do with it
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
            # Do nothing if queue empty
            except queue.Empty:
                pass

            # give the CPU a break
            time.sleep(0.05)

    def process(self):
        """
        Main processor for SB9600 and SBEP messages
        """

        while self.doProcess:

            # block until we have a message (this runs in a thread so that's okay)
            msg = self.rxMsgQueue.get()

            #self.logger.logVerbose("Processing {}".format(hexlify(msg," ")))

            # throw away invalid messages
            if len(msg) < 5:
                self.logger.logWarn("Skipping invalid message with length {}".format(len(msg)))
                continue

            # Handle SBEP first
            if self.inSBEP:
                # reset
                self.inSBEP = False
                # Process
                self.processSBEP(msg)

            # Handle SB9600
            else:

                # See if there are multiple messages combined and get them one at a time
                if len(msg) > 5:
                    while len(msg) > 5:
                        # Get the next message
                        curMsg = msg[0:5]
                        # Process it
                        data = self.bus.sb9600_decode(curMsg)
                        if data:
                            # Get message content
                            addr = data["address"]
                            param1 = data["param1"]
                            param2 = data["param2"]
                            func = data["function"]
                            # Process
                            self.processSB9600(addr, param1, param2, func)
                        else:
                            print("SB9600 CRC failure")
                        # Trim that message from the array and repeat
                        msg = msg[5:]

                        # If we were commanded to go into SBEP and we have more data remaining, process it as SBEP now so we don't miss it
                        if self.inSBEP and len(msg) > 5:
                            self.inSBEP = False
                            length = self.processSBEP(msg)
                            msg = msg[length:]

                # If we only had one message, or we have one left, process it
                if len(msg) == 5:
                    # Decode and process the message
                    data = self.bus.sb9600_decode(msg)
                    if data:
                        # Get message content
                        addr = data["address"]
                        param1 = data["param1"]
                        param2 = data["param2"]
                        func = data["function"]
                        # Process
                        self.processSB9600(addr, param1, param2, func)
                    else:
                        print("SB9600 CRC failure")

            # Update status if we're past the timer
            if time.time() > self.statusTimer + 0.1:
                if self.newStatus:
                    self.newStatus = False
                    self.updateStatus()
        

    def updateStatus(self):
        """
        Call the status callback with the index of the radio
        """
        self.statusCallback(self.index)

    def transmit(self, transmit):
        """
        Start or stop PTT to the radio

        Args:
            transmit (bool): PTT state
        """
        if transmit:
            self.sendButton(self.O5Address.button_map['ptt'], 0x01)
        else:
            self.sendButton(self.O5Address.button_map['ptt'], 0x00)

    def changeChannel(self, down):
        """
        Change channel up or down, depending on direction

        Args:
            down (bool): Move channel down, otherwise channel moves up
        """
        if down:
            self.sendButton(self.O5Address.button_map['knob_chan'], 0xFF)
        else:
            self.sendButton(self.O5Address.button_map['knob_chan'], 0x01)

    def toggleSoftkey(self, idx):
        """
        Press softkey button

        Args:
            idx (int): 1-5, softkey number
        """
        self.pressButton(self.O5Address.button_map["btn_key_{}".format(idx)], 0.05)

    def leftArrow(self):
        """
        Presses left arrow button
        """
        self.pressButton(self.O5Address.button_map["btn_dp_lf"], 0.05)

    def rightArrow(self):
        """
        Presses right arrow button
        """
        self.pressButton(self.O5Address.button_map["btn_dp_rg"], 0.05)

    def processSBEP(self, msg):
        """
        Main SBEP message processor

        Args:
            msg (byte[]): message array of bytes
        """

        self.logger.logDebug("SBEP msg  {}".format(hexlify(msg, " ")))
        self.logger.logDebug("SBEP text {}".format(msg.decode('ascii','ignore')))

        # get important bits
        address = msg[0]
        subaddr = msg[1]
        length = msg[2]
        opcode = msg[4]          
        # Get data based on length
        data = msg[8:length+2]

        # Get total msg length in bytes for return to processing function
        totalLength = length + 10

        # Display Address
        if address == 0x1f:
            # get display subdevice
            subdev = msg[6]
            # Handle based on display subdevice
            if subdev == self.O5Address.display_subdevs['text_zone']:
                newText = data.rstrip().decode('ascii')
                if newText != self.zoneText and not any(s in newText for s in self.O5Address.ignored_strings):
                    self.zoneText = newText
                    self.newStatus = True
                    self.logger.logVerbose("Got new zone text: {}".format(newText))
                return totalLength
            elif subdev == self.O5Address.display_subdevs['text_channel']:
                newText = data.rstrip().decode('ascii')
                if newText != self.chanText and not any(s in newText for s in self.O5Address.ignored_strings):
                    self.chanText = newText
                    self.newStatus = True
                    self.logger.logVerbose("Got new channel text: {}".format(newText))
                return totalLength
            elif subdev == self.O5Address.display_subdevs['text_softkeys']:
                self.softkeys = data.decode('ascii').rstrip().rstrip('\x00').split('^')[1:6]
                self.newStatus = True
                self.updateSoftkeys()
                self.logger.logVerbose("Got new softkeys: {}".format(self.softkeys))
                return totalLength

        # Display icon update 
        elif address == 0xf4:
            iconAddr = msg[3]
            # get icon
            icon = self.getDisplayIcon(msg[3])
            # get state
            if opcode == 0x00:
                state = False
            else:
                state = opcode
            # Update proper state
            # TODO: detect proper softkey for each status icon and update its status
            if iconAddr == self.O5Address.display_icons['scan']:
                self.logger.logVerbose("Got new scanning state: {}".format(state))
                if state != self.scanning:
                    self.scanning = state
                    self.updateSoftkeys()
                    self.newStatus = True
                return totalLength
            elif iconAddr == self.O5Address.display_icons['low_power']:
                self.logger.logVerbose("Got new lowpower state: {}".format(state))
                if state != self.lowpower:
                    self.lowpower = state
                    self.updateSoftkeys()
                    self.newStatus = True
                return totalLength
            elif iconAddr == self.O5Address.display_icons['monitor']:
                self.logger.logVerbose("Got new monitor state: {}".format(state))
                if state != self.monitor:
                    self.monitor = state
                    self.updateSoftkeys()
                    self.newStatus = True
                return totalLength
            elif iconAddr == self.O5Address.display_icons['direct']:
                self.logger.logVerbose("Got new direct state: {}".format(state))
                if state != self.direct:
                    self.direct = state
                    self.updateSoftkeys()
                    self.newStatus = True
                return totalLength

            # Use amber LED as a redundant RX state indicator
            elif iconAddr == self.O5Address.display_icons['led_amber']:
                if state and self.state != RadioState.Receiving:
                    self.state = RadioState.Receiving
                    self.newStatus = True

            # print if we don't actually know what the icon is
            #self.printMsg("SBEP Icon","{} ({}) icon {}".format(icon, hex(msg[3]), state))
            return totalLength

        # Fallback to printing raw message
        else:
            #print("RECVD<: SBEP decoded")
            #print("        Raw Msg: {}".format(hexlify(msg, ' ')))
            #print("        Address: {}, Subaddr: {}, Length: {}, Opcode: {}".format(hex(address), hex(subaddr), length, hex(opcode)))
            return totalLength

    def processSB9600(self, address, param1, param2, function):
        """
        Main handler for all incoming SB9600 messages from the radio

        Args:
            address (byte): Address byte
            param1 (byte): Parameter 1 byte
            param2 (byte): Parameter 2 byte
            function (byte): Function byte
        """

        self.logger.logDebug("SB9600 msg {} {} {} {}".format(hex(address),hex(param1),hex(param2),hex(function)))

        if address == 0x00:
            """
            Broadcast Module Address
                handles transition to SBEP and overall channel states
            """

            # SBEP command
            if function == 0x06:
                # Put us into SBEP mode
                self.inSBEP = True

                # Get baudrate
                if param1 == 0x12:
                    self.sbepSpeed = 9600
                else:
                    self.sbepSpeed = param1

                self.logger.logDebug("Entering SBEP mode at {} baud".format(self.sbepSpeed))

                return

            # Channel state command
            elif function == 0x0A:
                # Monitor mode
                if param1 == 0x01:
                    if param2 == 0x01:
                        if not self.monitor:
                            #self.monitor = True
                            #self.newStatus = True
                            pass
                        return
                    else:
                        if self.monitor:
                            #self.monitor = False
                            #self.newStatus = True
                            pass
                        return

                # TX mode
                elif param1 == 0x03:
                    if param2 == 0x01:
                        if self.state != RadioState.Transmitting:
                            self.state = RadioState.Transmitting
                            self.newStatus = True
                        return
                    else:
                        # Change to idle as long as we're not receiving
                        if (self.state != RadioState.Receiving) and (self.state != RadioState.Idle):
                            self.state = RadioState.Idle
                            self.newStatus = True
                        return

            # Not sure what this is, happens on startup
            elif function == 0x3b:
                return
            
            # Fallback for unknown message
            else:
                self.printMsg("Unknown","Unknown broadcast message function {}: params {}, {}".format(hex(function),hex(param1),hex(param2)))
                return

        if address == 0x05:
            """
            Front Panel Module
                handles buttons and illumination. Icons and text are handled via SBEP
            """

            # button / knob
            if function == 0x57:
                # lookup button
                btn = self.getButton(param1)
                if "knob" in btn:
                    # Knob CW/CCW
                    return
                else:
                    if param2 == 0x01:
                        # Button pressed
                        return
                    else:
                        # Button released
                        return

            # backlighting / illumination
            elif function == 0x58:
                # display BL
                if param1 == 0x02:
                    # Display backlight update
                    return
                elif param1 == 0x03:
                    # Button backlight update
                    return

        if address == 0x01:
            """
            Radio Module
                handles audio and channel states
            """

            # not sure, happens on startup
            if function == 0x15:
                return

            # not sure, happens during TX
            elif function == 0x19:
                return

            # audio device
            if function == 0x1D:
                if param2 == 0x01:
                    # Audio output unmuted
                    return
                elif param2 == 0x00:
                    # Audio output muted
                    return

            # channel state?
            elif function == 0x1e:
                # Channel idle
                if param1 == 0x00 and param2 == 0x00:
                    # Change to idle as long as we're not transmitting and not already idle
                    if (self.state != RadioState.Idle) and (self.state != RadioState.Transmitting):
                        self.state = RadioState.Idle
                        self.newStatus = True
                    return
                # Channel RX
                if param1 == 0x00 and param2 == 0x03:
                    if self.state !=RadioState.Receiving:
                        self.state = RadioState.Receiving
                        self.newStatus = True
                    return
                # This one can pop up when we're scanning and there's a nuisance deleted channel with activity
                # we ignore it for now
                if param1 == 0x00 and param2 == 0x01:
                    return

                # Fallback
                else:
                    self.printMsg("Channel","Unknown state: {} {}".format(hex(param1),hex(param2)))
                    return

            # channel change cmd device?
            elif function == 0x1f:
                # Channel change command
                return

            # this one seems to follow the channel RX state, but doesn't always fire
            elif function == 0x23:
                return

            # not sure, happens during TX
            elif function == 0x3c:
                return

            # channel change ack device?
            elif function == 0x60:
                # Channel change ACK
                return

            elif (function == 0x1a) and (param1, param2 == 0x00):
                # Ignore this one, it gets spammed a bunch and I don't know what it is
                return

            # fallback
            else:
                self.printMsg("Unknown","Unknown message for radio module (0x01): func {}, params {} {}".format(hex(function),hex(param1),hex(param2)))
                return

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
            for key, value in self.O5Address.button_map.items():
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

    def pressButton(self, code, duration):
        """
        Press a button for a specified duration, holding down BUSY so we don't get any weirdness

        Args:
            code (byte): button opcode
            duration (float): duration to press in seconds
        """
        self.sendButton(code, 0x01)
        time.sleep(duration)
        self.sendButton(code, 0x00)

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
            for key, value in self.O5Address.display_subdevs.items():
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
            for key, value in self.O5Address.display_icons.items():
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