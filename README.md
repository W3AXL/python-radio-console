# Python Radio Console
Control Motorola radios with your computer and some basic hardware!

![Console Screenshot](/media/window-screenshot.png)

## Requirements
### Python
Python 3.x installation with the following libraries:
* [`pyserial`](https://pythonhosted.org/pyserial)
* [`sounddevice`](https://python-sounddevice.readthedocs.io)
* [`websockets`](https://websockets.readthedocs.io)
* [`colorama`](https://pypi.org/project/colorama)

The rest of the needed libraries should be included with a base python install.

## Setup
1. Clone the repo to a local directory of your choice. 
2. Open a terminal window in this directory and create a new python virtualenv using `python -m venv .`
3. Install the required libraries using pip and the requirements.txt file using `python -m pip install -r requirements.txt`

## Hardware
![Hardware Block Diagram](/media/console-hardware-diagram.png)

You'll need at least one radio to control before you can get the server up and running.

### Control interface - SB9600
Most modern Motorola radios should have an SB9600 bus with four signals: BUS+, BUS-, BUSY, and ground. You'll need a RIB (radio interface box) to translate these signals into standard serial. If you're having issues getting the radio to connect, try switching the BUSY signal for the /BUSY signal (active low vs active high) as the RIB output has both available.

### Audio Interface
I used a cheapo USB soundcard with mic & headphone plugs. Wire RX audio from the radio's accessory connector (for example, RX_FILT_AUD on the XTL5000) to the mic input, and wire TX audio from the soundcard headphone jack to the MIC line that will work with your radio.

#### A note on mic audio input
Currently this console controls PTT by sending the SB9600 command for a PTT button press. On the radio I tested with (an XTL5000 with an O5 head), mic audio could only be input to the radio via the front microphone connector. The MIC_IN pin on the radio's TIB was disabled, and AUX_MIC/AUX_TX require a hardware signal (AUX_PTT) to be triggered before they get enabled.

## Configuration File
Radio connections are defined in a JSON config file.

TODO: Finish this readme
