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


    def __init__(self, index, comPort, headType, statusCallback):
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
        self.scanning = False
        self.talkaround = False
        self.monitor = False
        self.lowpower = False
        self.muted = True

        # Logger
        self.logger = Logger()


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
        self.msgQueue = queue.Queue()

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
            # Read message if there's one waiting
            if self.bus.ser.in_waiting > 0:
                # Read message
                msg = self.bus.read(self.bus.ser.in_waiting)
                # Put into queue
                self.msgQueue.put_nowait(msg)
                #self.logger.logInfo("Got msg {}".format(hexlify(msg," ")))

    def process(self):
        """
        Main processor for SB9600 and SBEP messages
        """

        while self.doProcess:

            # block until we have a message (this runs in a thread so that's okay)
            msg = self.msgQueue.get()

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

                # See if there are multiple messages combined and get them first
                if len(msg) > 5 and len(msg) % 5 == 0:
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

    def toggleMonitor(self):
        """
        Monitor button
        """
        # Press nuis button
        self.pressButton(self.O5Address.button_map["btn_key_1"], 0.1) 

    def nuisanceDelete(self):
        """
        Nuisance delete button
        """
        # Press nuis button
        self.pressButton(self.O5Address.button_map["btn_key_2"], 0.1)

    def togglePower(self):
        """
        Power button
        """
        # Press nuis button
        self.pressButton(self.O5Address.button_map["btn_key_3"], 0.1)

    def toggleScan(self):
        """
        Change state of scan by sending softkey button to toggle
        """
        # Press scan button
        self.pressButton(self.O5Address.button_map['btn_key_4'],0.1)

    def toggleDirect(self):
        """
        DIR button
        """
        # Press scan button
        self.pressButton(self.O5Address.button_map['btn_key_5'],0.1)

    def processSBEP(self, msg):
        """
        Main SBEP message processor

        Args:
            msg (byte[]): message array of bytes
        """

        #self.logger.logInfo("SBEP msg {}".format(hexlify(msg)))

        # get important bits
        address = msg[0]
        subaddr = msg[1]
        length = msg[2]
        opcode = msg[4]          
        # Get data based on length
        data = msg[8:length+2]

        # Display Address
        if address == 0x1f:
            # get display subdevice
            subdev = msg[6]
            # Handle based on display subdevice
            if subdev == self.O5Address.display_subdevs['text_zone']:
                newText = data.rstrip().decode('ascii')
                if newText != self.zoneText:
                    self.zoneText = newText
                    self.newStatus = True
                return
            elif subdev == self.O5Address.display_subdevs['text_channel']:
                newText = data.rstrip().decode('ascii')
                if newText != self.chanText:
                    self.chanText = newText
                    self.newStatus = True
                return

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
            if iconAddr == self.O5Address.display_icons['scan']:
                if state != self.scanning:
                    self.scanning = state
                    self.newStatus = True
                return
            elif iconAddr == self.O5Address.display_icons['low_power']:
                if state != self.lowpower:
                    self.lowpower = state
                    self.newStatus = True
                return
            elif iconAddr == self.O5Address.display_icons['monitor']:
                if state != self.monitor:
                    self.monitor = state
                    self.newStatus = True
                return
            elif iconAddr == self.O5Address.display_icons['direct']:
                if state != self.talkaround:
                    self.talkaround = state
                    self.newStatus = True
                return


            # print if we don't actually know what the icon is
            #self.printMsg("SBEP Icon","{} ({}) icon {}".format(icon, hex(msg[3]), state))
            return

        # Fallback to printing raw message
        else:
            #print("RECVD<: SBEP decoded")
            #print("        Raw Msg: {}".format(hexlify(msg, ' ')))
            #print("        Address: {}, Subaddr: {}, Length: {}, Opcode: {}".format(hex(address), hex(subaddr), length, hex(opcode)))
            return

    def processSB9600(self, address, param1, param2, function):
        """
        Main handler for all incoming SB9600 messages from the radio

        Args:
            address (byte): Address byte
            param1 (byte): Parameter 1 byte
            param2 (byte): Parameter 2 byte
            function (byte): Function byte
        """

        #self.logger.logInfo("SB9600 msg {} {} {} {}".format(hex(address),hex(param1),hex(param2),hex(function)))

        if address == 0x00:
            """
            Broadcast Module Address
                handles transition to SB9600 and overall channel states
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

                return

            # Channel state command
            elif function == 0x0A:
                # Monitor mode
                if param1 == 0x01:
                    if param2 == 0x01:
                        if not self.monitor:
                            self.monitor = True
                            self.newStatus = True
                        return
                    else:
                        if self.monitor:
                            self.monitor = False
                            self.newStatus = True
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
        Send a button command to the radio

        Args:
            code (byte): button code
            value (byte): value to send
        """
        try:
            self.bus.sb9600_send(self.bus.sbep_modules['PANEL'], code, value, 0x57)
        except RuntimeError as ex:
            self.logger.logWarn("Couldn't verify button message was sent properly! Maybe other data on bus?")

    def pressButton(self, code, duration):
        """
        Press a button for a specified duration

        Args:
            code (byte): button opcode
            duration (float): duration to press in seconds
        """
        try:
            self.bus.sb9600_send(self.bus.sbep_modules['PANEL'], code, 0x01, 0x57)
            time.sleep(duration)
            self.bus.sb9600_send(self.bus.sbep_modules['PANEL'], code, 0x00, 0x57)
        except RuntimeError as ex:
            self.logger.logWarn("Couldn't verify button message was sent properly! Maybe other data on bus?")

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