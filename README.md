# Python Radio Console
Control Motorola radios with your computer and some basic hardware!

![Console Screenshot](/media/screenshot-v2.png)

**Important note: this system is in super-duper early alpha stage. Don't expect anything to work perfectly. Please for the love of god don't use this in any type of mission-critical application.** Submit new issues via Github for anything you manage to break, and I'll do my best to try and fix it. Help would be greatly appreciated, and anyone who's a better programmer than I am please feel free to submit pull requests.

## Hardware Overview

![Hardware Block Diagram](/media/system-diagram-v2.drawio.png)

The console uses dead-simple interfaces for audio & communication - USB audio dongles, serial ports, and network connections. The server can be run on the same device as the client, or they can be separated by any distance so long as they can talk to each other over a network.

## Security Note!
Currently, there is no authentication implemented in the console system. Operating radio daemons over the public internet is not recommended. If remote operation is desired (and it's absolutely possible) VPN use to connect sites is highly recommended.

## Requirements
For a basic console setup, the following is required:
* Python 3.9 Installation
* Modern Web Browser (Chrome, Firefox, Edge)
* USB Audio Interface (cheap CM108-based adapters will work fine, or something fancy like a Focusrite or Behringer audio interface)
* Motorola XTL or XPR series radio (future work will include more radios)
   * **For XTL series radios:** Motorola RIB (Radio Interface Box) for SB9600 to Serial communications and a working serial port (USB or hardware). Note that currently, only O5/M5 series control heads are supported and mic audio must be passed via the front microphone port.
   * **For XPR series radios:** Rear accessory connector for USB and audio communication.

## Setup

[See SETUP.md for information on setup and configuration](https://github.com/W3AXL/python-radio-console/SETUP.md)

## Known Issues

* [#2](https://github.com/W3AXL/python-radio-console/issues/2) - Connecting a second instance of the console web GUI to the server will cause lots of issues. This will be fixed eventually to kick off the old session when the new one connects.
* [#5](https://github.com/W3AXL/python-radio-console/issues/5) - You can only PTT with spacebar when you have the console window in focus. This is a limitation of web browsers (you can't capture keypresses that aren't for that window). Future plans include joystick/controller support to interface with a footswitch or something which *should* be possible even when the window isn't in focus.
* [#33](https://github.com/W3AXL/python-radio-console/issues/33) - WebRTC memory leak exists when running radio daemons. This is an underlying issue with the PyAV audio library used by WebRTC. Workaround is to restart radio daemons to clear memory usage.