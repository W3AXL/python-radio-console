# Base libraries
import sys
import os

# QT stuff
from PySide6.QtWidgets import QApplication, QMainWindow, QMessageBox, QFileDialog, QTreeWidgetItem, QAbstractItemView
from PySide6.QtCore import QThread, QThreadPool, Signal
from PySide6.QtGui import QActionGroup, QAction

# Material design stylesheet
from qt_material import apply_stylesheet

# UI window
from serverwindow import Ui_ServerWindow

# Sound stuff
import sounddevice as sd

# Serial stuff
import serial
import serial.tools.list_ports

# Radio class
from radioClass import Radio, RadioStatus

# Used for saving config
import json

# Config class (saved/loaded to JSON)
class Config():
    # Init
    def __init__(self):
        # List of radio configs
        self.RadioList = []

# ServerWindow class for main server window
class ServerWindow(QMainWindow):

    # Init function
    def __init__(self):

        # Config variable
        self.config = Config()

        # Setup window
        super(ServerWindow, self).__init__()
        self.ui = Ui_ServerWindow()
        self.ui.setupUi(self)

        # Disable resize
        self.setFixedSize(self.width(), self.height())

        # Init variables
        self.editing = False
        self.editingIndex = None
        self.editingRadio = None
        self.unsavedChanges = False

        # Setup UI elements
        self.setupUI()

    # Connect UI elements to functions
    def setupUI(self):
        # Setup signals for menu items
        self.ui.aMenuQuit.triggered.connect(self.quit)              # Quit program
        self.ui.aMenuSaveConfig.triggered.connect(self.saveConfig)  # Save config file
        self.ui.aMenuOpenConfig.triggered.connect(self.openConfig)  # Open config file

        # Setup buttons
        self.ui.fAddRadio.clicked.connect(self.addNewRadio)
        self.ui.fEditRadio.clicked.connect(self.editRadio)

        # Setup radio list
        self.ui.fRadioList.currentItemChanged.connect(self.selectRadio)
        self.ui.fRadioList.setColumnWidth(0,140)
        self.ui.fRadioList.setColumnWidth(1,200)
        self.ui.fRadioList.setSelectionMode(QAbstractItemView.SingleSelection)

        # Setup signals for radio editing GUI
        self.ui.fEditRadioSigMode.currentTextChanged.connect(self.selectSignalMode)
        self.ui.fEditRadioCtrlMode.currentTextChanged.connect(self.selectControlMode)

        # Populate valid control modes
        for mode in Radio.controlModes:
            self.ui.fEditRadioCtrlMode.addItem(mode)

        # Populate valid signalling modes
        for mode in Radio.signallingModes:
            self.ui.fEditRadioSigMode.addItem(mode)

        # Populate sound devices
        self.getSoundDevices()

        # Populate serial devices
        self.getSerialDevices()

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

    def getSerialDevices(self):
        print("Querying serial devices")
        # Get ports
        ports = serial.tools.list_ports.comports()
        # Iterate and add to dropdown
        for port in ports:
            self.ui.fEditRadioCtrlPort.addItem(port.name)

    # Update the list of radios from the radio list object 
    def updateRadioList(self):
        # clear list
        self.ui.fRadioList.clear()
        # update list
        for radio in self.config.RadioList:
            # Create item for radio
            radioItem = QTreeWidgetItem()
            radioItem.setText(0, radio.name)
            radioItem.setText(1, radio.desc)
            radioStatus, statusText = radio.getStatus()
            radioItem.setText(2, statusText)
            self.ui.fRadioList.addTopLevelItem(radioItem)

    # Enable/disable radio edit dialog
    def enableRadioEdit(self, enabled):
        # status variable
        self.editing = enabled
        # enable edit boxes
        self.ui.fEditRadioName.setEnabled(enabled)
        self.ui.fEditRadioDesc.setEnabled(enabled)
        self.ui.fEditRadioCtrlMode.setEnabled(enabled)
        # Enable control port selection if we're in a serial-based control mode (i.e. not a soundcard mode or "None")
        if enabled:
            self.selectControlMode()
        else:
            self.ui.fEditRadioCtrlPort.setEnabled(False)
        # Enable the rest of the dropdowns
        self.ui.fEditRadioTxAud.setEnabled(enabled)
        self.ui.fEditRadioRxAud.setEnabled(enabled)
        self.ui.fEditRadioSigMode.setEnabled(enabled)
        self.ui.fEditRadioSigId.setEnabled(enabled)
        # disable selection & add, change edit to save
        self.ui.fRadioList.setEnabled(not enabled)
        self.ui.fAddRadio.setEnabled(not enabled)
        self.ui.fDeleteRadio.setEnabled(not enabled)
        if enabled:
            self.ui.fEditRadio.setText("Save")
        else:
            self.ui.fEditRadio.setText("Edit")

    # Add radio
    def addNewRadio(self):
        # Add a new radio entry to the radio list
        newRadio = Radio("New Radio")
        self.config.RadioList.append(newRadio)
        # Update radio list
        self.updateRadioList()
        # Unsaved changes
        self.unsavedChanges = True

    # New radio in list selected
    def selectRadio(self):
        if self.ui.fRadioList.currentItem() != None:
            # get the currently selected item
            selectedName = self.ui.fRadioList.currentItem().text(0)
            # get the associated radio from the list
            for radio in self.config.RadioList:
                if radio.name == selectedName:
                    break
            else:
                raise ValueError("Selected radio not present in radio list")
            # populate the radio info pane
            self.populateRadioFields(radio)
            # Update radio status
            radioStatus, statusString = radio.getStatus()
            self.ui.lblSelectedRadioStatus.setText(statusString)
            # enable connect button if not connected
            if radio.status != RadioStatus.Idle or radio.status != RadioStatus.Transmitting or radio.status != RadioStatus.Receiving:
                self.ui.fConnectRadio.setEnabled(True)
                self.ui.fDisconnectRadio.setEnabled(False)
            else:
                self.ui.fConnectRadio.setEnabled(False)
                self.ui.fDisconnectRadio.setEnabled(True)
        else:
            # Clear radio fields
            self.populateRadioFields(None)
            # Clear radio status
            self.ui.lblSelectedRadioStatus.setText("No Radio Selected")
            # Disabled connect/disconnect buttons
            self.ui.fConnectRadio.setEnabled(False)
            self.ui.fDisconnectRadio.setEnabled(False)

    # Edit radio button clicked
    def editRadio(self):
        # Start Editing
        if not self.editing:
            # get the selected radio
            radioName = self.ui.fRadioList.currentItem().text(0)
            print("Editing radio {}".format(radioName))
            # get the associated radio from the list
            for index, radio in enumerate(self.config.RadioList):
                if radio.name == radioName:
                    break
            else:
                raise ValueError("Selected radio not present in radio list")
            # Update the currently editing variables
            self.editingIndex = index
            self.editingRadio = self.config.RadioList[index]
            # enable editing
            self.enableRadioEdit(True)
        # Save edits
        else:
            # check if name already exists and isn't the one we're editing
            newName = self.ui.fEditRadioName.text()
            for index, radio in enumerate(self.config.RadioList):
                if radio.name == newName and index != self.editingIndex:
                    self.showError("Radio name already exists!")
                    return
            # create a new radio object
            newRadio = Radio(
                name=newName,
                desc=self.ui.fEditRadioDesc.text(),
                ctrlMode=self.ui.fEditRadioCtrlMode.currentText(),
                ctrlPort=self.ui.fEditRadioCtrlPort.currentText(),
                txDev=self.ui.fEditRadioTxAud.currentText(),
                rxDev=self.ui.fEditRadioRxAud.currentText(),
                signalMode=self.ui.fEditRadioSigMode.currentText(),
                signalId=self.ui.fEditRadioSigId.text()
            )
            # update the list with the new object
            self.config.RadioList[self.editingIndex] = newRadio
            # reset the editing variables
            self.editingIndex = None
            self.editingRadio = None
            # update the list of radios
            self.updateRadioList()
            # disable editing
            self.enableRadioEdit(False)
            # unsaved changes
            self.unsavedChanges = True

    # Function triggered when signalling mode is changed in dropdown
    def selectSignalMode(self):
        if self.editing:
            if self.ui.fEditRadioSigMode.currentText() == "None":
                self.ui.fEditRadioSigId.setEnabled(False)
            else:
                self.ui.fEditRadioSigId.setEnabled(True)

    # Function is called when control mode is changed in dropdown
    def selectControlMode(self):
        if self.editing:
            currentMode = self.ui.fEditRadioCtrlMode.currentText()
            if "Soundcard" not in currentMode and "None" not in currentMode:
                self.ui.fEditRadioCtrlPort.setEnabled(True)
            else:
                self.ui.fEditRadioCtrlPort.setEnabled(False)

    # populate radio info UI elements
    def populateRadioFields(self, radio):
        if radio != None:
            self.ui.fEditRadioName.setText(radio.name)
            self.ui.fEditRadioDesc.setText(radio.desc)
            self.ui.fEditRadioCtrlMode.setCurrentText(radio.ctrlMode)
            self.ui.fEditRadioCtrlPort.setCurrentText(radio.ctrlPort)
            self.ui.fEditRadioTxAud.setCurrentText(radio.txDev)
            self.ui.fEditRadioRxAud.setCurrentText(radio.rxDev)
            self.ui.fEditRadioSigMode.setCurrentText(radio.sigMode)
            self.ui.fEditRadioSigId.setText(radio.sigId)
        else:
            self.ui.fEditRadioName.setText("")
            self.ui.fEditRadioDesc.setText("")
            self.ui.fEditRadioCtrlMode.setCurrentText("")
            self.ui.fEditRadioCtrlPort.setCurrentText("")
            self.ui.fEditRadioTxAud.setCurrentText("")
            self.ui.fEditRadioRxAud.setCurrentText("")
            self.ui.fEditRadioSigMode.setCurrentText("")
            self.ui.fEditRadioSigId.setText("")

    # Load config from json
    def openConfig(self):
        # Check if we have unsaved changed
        if self.unsavedChanges:
            answer = self.showYesNoCancel("Unsaved config changes", "Warning", "Save existing config file?", QMessageBox.Warning)
            if answer == QMessageBox.Yes:
                # if we don't actually save, don't do anything further
                if not self.saveConfig():
                    return False
            elif answer == QMessageBox.Cancel:
                return False

        # Show dialog for open
        filename, types = QFileDialog.getOpenFileName(self, caption='Open Config File', filter="Config files (*.cfg)")

        # return if no file selected
        if not filename:
            return False
        
        # Open and read file
        with open(filename, 'r') as inp:
            try:
                # load JSON into dict
                configDict = json.load(inp)

                # create new blank config
                self.config = Config()

                # iterate through radios in dict
                for radioDict in configDict["RadioList"]:
                    self.config.RadioList.append(Radio.decodeConfig(radioDict))

                # print on success
                print("Opened config file: {}".format(filename))
                self.updateRadioList()

                # return
                return True
            except Exception as ex:
                self.showError("Error loading config!",ex.args[0])
                return False
            
    # Save config to json
    def saveConfig(self):
        # Show save dialog
        filename, types = QFileDialog.getSaveFileName(self, caption='Save Config File', filter="Config files (*.cfg)")

        # return if no file selected
        if not filename:
            return False
        
        # Create dict from config
        configDict = {
            "RadioList": []
        }
        for radio in self.config.RadioList:
            radioDict = radio.encodeConfig()
            configDict["RadioList"].append(radioDict)
        
        # Save as JSON
        with open(filename, 'w') as outp:
            json.dump(configDict, outp)
        
        # Updated unsaved changes
        self.unsavedChanges = False
        # Log
        print("Saved config file: {}".format(filename))
        # return
        return True

    # Show error dialog
    def showError(self, message, submessage=None, confirm=False):
        msgBox = QMessageBox()
        msgBox.setIcon(QMessageBox.Critical)
        msgBox.setWindowTitle("Error")
        msgBox.setText(message)
        msgBox.setInformativeText(submessage)
        if confirm:
            msgBox.setStandardButtons(QMessageBox.Ok | QMessageBox.Cancel)
            msgBox.setDefaultButton(QMessageBox.Cancel)
        returnval = msgBox.exec()
        if returnval == QMessageBox.Ok:
            return True
        else:
            return False

    # Show warning dialog
    def showWarn(self, message, submessage=None, confirm=False):
        msgBox = QMessageBox()
        msgBox.setIcon(QMessageBox.Warning)
        msgBox.setWindowTitle("Warning")
        msgBox.setText(message)
        msgBox.setInformativeText(submessage)
        if confirm:
            msgBox.setStandardButtons(QMessageBox.Ok | QMessageBox.Cancel)
            msgBox.setDefaultButton(QMessageBox.Cancel)
        returnval = msgBox.exec()
        if returnval == QMessageBox.Ok:
            return True
        else:
            return False

    # Show yes/no/cancel
    def showYesNoCancel(self, message, title, submessage=None, icon=None):
        msgBox = QMessageBox()
        msgBox.setIcon(icon)
        msgBox.setWindowTitle(title)
        msgBox.setText(message)
        msgBox.setInformativeText(submessage)
        msgBox.setStandardButtons(QMessageBox.Yes | QMessageBox.No | QMessageBox.Cancel)
        msgBox.setDefaultButton(QMessageBox.Cancel)
        return msgBox.exec()

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