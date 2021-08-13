from interface import sb9600
import threading

from binascii import hexlify

from radioState import RadioState

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


    def connect(self):
        """
        Connect and initialize radio
        """

        # Create and flush serial bus
        self.bus = sb9600.Serial(self.comPort)
        self.bus.ser.flush()

        # SBEP mode switch var
        self.inSBEP = False
        self.sbepSpeed = 0

        # Create listener thread and start it
        self.listenerThread = threading.Thread(target=self.listen, daemon=True)
        self.listenerThread.start()

        # Reset the radio to init all the statuses
        self.bus.sb9600_reset()

        # Update radio state
        self.state = RadioState.Idle


    def listen(self):
        """
        Main listener for SB9600 and SBEP messages

        Args:
            callback (function): [description]
        """

        while True:
            # Read message if there's one waiting
            if self.bus.ser.in_waiting > 0:
                # Read message
                msg = self.bus.read(self.bus.ser.in_waiting)

                # Handle SBEP first
                if self.inSBEP:
                    # reset
                    self.inSBEP = False
                    # Process
                    self.processSBEP(msg)

                # Handle SB9600
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

    def updateStatus(self):
        self.statusCallback(self.index)

    def processSBEP(self, msg):
        """
        Main SBEP message processor

        Args:
            msg (byte[]): message array of bytes
        """

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
                self.zoneText = data.rstrip().decode('ascii')
                self.updateStatus()
                return
            elif subdev == self.O5Address.display_subdevs['text_channel']:
                self.chanText = data.rstrip().decode('ascii')
                self.updateStatus()
                return

        # Display icon update 
        elif address == 0xf4:
            iconAddr = msg[3]
            # get icon
            icon = self.getDisplayIcon(msg[3])
            # get state
            if opcode == 0x01:
                state = True
            else:
                state = False
            # Update proper state
            if iconAddr == self.O5Address.display_icons['scan']:
                self.scanning = state
                self.updateStatus()
                return
            elif iconAddr == self.O5Address.display_icons['low_power']:
                self.lowpower = state
                self.updateStatus()
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
                        self.monitor = True
                        self.updateStatus()
                        return
                    else:
                        self.monitor = False
                        self.updateStatus()
                        return

                # TX mode
                elif param1 == 0x03:
                    if param2 == 0x01:
                        self.state = RadioState.Transmitting
                        self.updateStatus()
                        return
                    else:
                        self.state = RadioState.Idle
                        self.updateStatus()
                        return
            
            # Fallback for unknown message
            else:
                #self.printMsg("Unknown","Unknown broadcast message function {}: params {}, {}".format(hex(function),hex(param1),hex(param2)))
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
                    self.state = RadioState.Idle
                    self.updateStatus()
                    return
                # Channel RX
                if param2 == 0x03:
                    self.state = RadioState.Receiving
                    self.updateStatus()
                    return
                else:
                    #self.printMsg("Channel","Unknown state: {} {}".format(hex(param1),hex(param2)))
                    return

            # channel change cmd device?
            elif function == 0x1f:
                # Channel change command
                return

            # channel change ack device?
            elif function == 0x60:
                # Channel change ACK
                return

            # fallback
            else:
                #self.printMsg("Unknown","Unknown message for radio module (0x01): func {}, params {} {}".format(hex(function),hex(param1),hex(param2)))
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

        print("{: >10} >>: {}".format(source, msg))