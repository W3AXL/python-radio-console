# Base libraries
import sys
import os

# QT stuff
from PySide6.QtWidgets import QApplication, QMainWindow
from PySide6.QtCore import QThread, QThreadPool, Signal
from PySide6.QtGui import QActionGroup, QAction

# Material design stylesheet
from qt_material import apply_stylesheet

# UI window
from serverwindow import Ui_ServerWindow

# Sound stuff
import sounddevice as sd

# Radio class
from radioClass import Radio

# ServerWindow class for main server window
class ServerWindow(QMainWindow):

    # Init function
    def __init__(self):

        # Setup window
        super(ServerWindow, self).__init__()
        self.ui = Ui_ServerWindow()
        self.ui.setupUi(self)

        # Disable resize
        self.setFixedSize(self.width(), self.height())

        # Setup UI elements
        self.setupUI()

    # Connect UI elements to functions
    def setupUI(self):
        # Setup signals for menu items
        self.ui.aMenuQuit.triggered.connect(self.quit)      # Quit program

        # Setup signals for buttons

        # Populate valid control modes
        for mode in Radio.controlModes:
            self.ui.fEditRadioCtrlMode.addItem(mode)

        # Populate sound devices
        self.getSoundDevices()

    def getSoundDevices(self):
        print("Querying sound devices")
        # Get available output devices
        devs = sd.query_devices()
        for dev in devs:
            devString = "{}: {}".format(dev['hostapi'],dev['name'])
            # output (TX device)
            if dev['max_output_channels'] > 0:
                self.ui.fEditRadioTxAud.addItem(devString)
            # input (RX device)
            if dev['max_input_channels'] > 0:
                self.ui.fEditRadioRxAud.addItem(devString)

    # Enable/disable radio edit dialog
    def enableRadioEdit(self, enabled):
        for item in self.ui.grdRadioSettings.children():
            item.setEnabled(enabled)

    # Add radio
    def addRadio(self):
        # Add a new entry to the item list
        newRadio = self.ui.fRadioListView.items

    # Quit
    def quit(self):
        # Exit application
        self.close()

# Start window on runtime start
if __name__ == "__main__":
    # setup app and window
    app = QApplication(sys.argv)
    window = ServerWindow()
    # apply material design stylesheet
    apply_stylesheet(app, theme='dark_blue.xml')
    # show window
    window.show()

    sys.exit(app.exec())