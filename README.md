# Python Radio Console
Control Motorola radios with your computer and some basic hardware!

![Console Screenshot](/media/window-screenshot.png)

**Important note: this system is in super-duper early alpha stage. Don't expect anything to work perfectly. Please for the love of god don't use this in any type of mission-critical application.** Submit new issues via Github for anything you manage to break, and I'll do my best to try and fix it. Help would be greatly appreciated, and anyone who's a better programmer than I am please feel free to submit pull requests.

## Hardware Overview

![Hardware Block Diagram](https://github.com/W3AXL/python-radio-console/blob/master/media/console-hardware-diagram.png)

The console uses dead-simple interfaces for audio & communication - USB audio dongles and serial ports. The server can be run on the same device as the client, or they can be separated by any distance so long as they can talk to each other over a network.

## Requirements
* Python 3.9 Installation
* Modern Web Browser (Chrome, Firefox, Edge)
* USB Audio Interface (cheap CM108-based adapters will work fine) or something fancy like a Focusrite or Behringer audio interface
* Motorola XTL or XPR series radio (future work will include more radios)
   * **For XTL series radios:** Motorola RIB (Radio Interface Box) for SB9600 to Serial communications and a working serial port (USB or hardware)
   * **For XPR series radios:** Rear accessory connector for USB and audio communication.

## Setup

[See the setup wiki page for the latest setup guide](https://github.com/W3AXL/python-radio-console/wiki/Setup)

## Known Issues

* [#2](https://github.com/W3AXL/python-radio-console/issues/2) - Connecting a second instance of the console web GUI to the server will cause lots of issues. This will be fixed eventually to kick off the old session when the new one connects.
* [#4](https://github.com/W3AXL/python-radio-console/issues/4) - The very end of outgoing transmissions can get cut off. This is due to latency between the client and the server, as well as audio device latency. Just make sure you give a pause before releasing PTT after finishing your transmission.
* [#5](https://github.com/W3AXL/python-radio-console/issues/5) - You can only PTT with spacebar when you have the console window in focus. This is a limitation of web browsers (you can't capture keypresses that aren't for that window). Future plans include joystick/controller support to interface with a footswitch or something which *should* be possible even when the window isn't in focus.
