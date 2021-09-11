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

(Note that the server & client machines can be the same - that's what I've been using for testing)

You'll need at least one radio to control before you can get the server up and running.

### Control interface - SB9600
Most modern Motorola radios should have an SB9600 bus with four signals: BUS+, BUS-, BUSY, and ground. You'll need a RIB (radio interface box) to translate these signals into standard serial. If you're having issues getting the radio to connect, try switching the BUSY signal for the /BUSY signal (active low vs active high) as the RIB output has both available.

*I've been working on a small board to do USB -> Serial -> SB9600 all in one compact package. More to come on that!*

### Audio Interface
I used a cheapo USB soundcard with mic & headphone plugs. Wire RX audio from the radio's accessory connector (for example, RX_FILT_AUD on the XTL5000) to the mic input, and wire TX audio from the soundcard headphone jack to the MIC line that will work with your radio.

#### A note on mic audio input
Currently this console controls PTT by sending the SB9600 command for a PTT button press. On the radio I tested with (an XTL5000 with an O5 head), mic audio could only be input to the radio via the front microphone connector. The MIC_IN pin on the radio's TIB was disabled, and AUX_MIC/AUX_TX require a hardware signal (AUX_PTT) to be triggered before they get enabled.

## Configuration File
Radio connections are defined in a JSON config file.

`config.json`:
```
{
    "RadioList": [
        {
            "name": "Desk VHF XTL",
            "desc": "VHF XTL5000",
            "ctrlMode": "SB9600-XTL-O",
            "ctrlPort": "COM2",
            "txDeviceIdx": 18,
            "rxDeviceIdx": 4,
            "sigMode": "MDC",
            "sigId": "4597"
        }
    ]
}
```

* `name`: Radio name. Shown in the console radio headers. Be sensible in your name lengths. There's no limit but it'll look bad.
* `desc`: Radio description. Not used anywhere yet, but might be at some point. Can be blank I suppose.
* `ctrlMode`: Radio control mode. Options are
  * **Implemented**:
    * `SB9600-XTL-O`: XTL5000 with O5 control head
  * **Planned**:
    * `SB9600-XTL-W`: XTL5000 with W9 control head
    * `SB9600-MCS-3`: MCS2000 with model 3 control head
    * `SB9600-AS-W`: Astro spectra with W9 control head (will probably just work with the XTL W profile)
    * `Soundcard-CM108`: CM108B/CM119 soundcard PTT control (for a generic radio. No control besides PTT will be available)
    * `Soundcard-VOX`: VOX-based PTT control. Same as above except relying on radio VOX circuit for PTT. Don't ever use this and I probably won't actually do it.
* `ctrlPort`: Serial port the above interface is connected to. `COMx` for Windows, `/dev/ttySx` for linux.
* `txDeviceIdx`: Index of the transmit audio device. Device indecies can be queried using `python server-runtime.py -ls`
* `rxDeviceIdx`: Index of the receive audio device. Indecies can be queried using the above command.
* `sigMode`: **To be implemented.** Planned encode/decode of MDC, QCII, DTMF, etc, without needing a radio to do it natively.
* `sigId`: **To be implemented.** ID for the above signalling mode.

To add more radios, just add more entries to the `RadioList`. Standard JSON formatting rules apply.

## HTTPS certificates for a locally-hosted server

The console client uses several new-ish javascript features, some of which require the webpage to be served over an SSL connection (in Chrome at least). It's annoying but I haven't found a workaround for this yet. The python server runtime starts its own https webserver, but it needs valid certificates otherwise the client-side portion will run into all kinds of issues.

[You can generate your own CA & certificate using this answer from StackOverflow.](https://stackoverflow.com/a/60516812/1842613) You'll have to overwrite the existing keys & certs in the `certs/` folder of this repo, but if you use the same names everything should "just work."

Once you go through these annoying steps, the https connection should then work without a bunch of errors and issues. There's probably a better and more secure way to do this but for now I've also included the CA & cert files I used. If you don't care too much about the security implications of trusting a random CA from a Github repo, you can trust the localCA.pem authority in Chrome/Windows and things should work with minimal effort.

## Running the Console

Okay, you've got your hardware set up, a configuration file made, and the certs for the console configured. How do we actually use the damn thing?

**Note: the console has so far only been tested with Windows 10 and Google Chrome, with everything running on the same machine. The python libraries used should be cross-platform but the software is super-early alpha and no guarantees are made.**

1. Activate the virtual environment you made above from a terminal prompt:  
`.\Scripts\activate` (Windows) or `source ./bin/activate` (Linux)
2. Start the console with your config file and specify ports for the webserver and client-to-server messaging service:  
`python server-runtime.py -c config.json -sp 9995 -wp 4443`
3. Navigate to `server.address:webport` (example: `localhost:4443`) and an empty console should load. If you've gotten this far, you're in the home stretch now.  

4. 

## Known Issues

* Connecting a second console webpage to the server will cause lots of issues. This will be fixed eventually to kick off the old session when the new one connects.
* RX audio occasionally pops & clicks. I think this is an issue with the mu-law encoding and not a buffer underrun issue. As far as I can tell at least.
* You can only PTT with spacebar when you have the console window in focus. This is a limitation of web browsers (you can't capture keypresses that aren't for that window). Future plans include joystick/controller support to interface with a footswitch or something which *should* be possible even when the window isn't in focus.
