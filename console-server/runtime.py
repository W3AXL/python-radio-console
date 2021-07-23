# Base libraries
import sys
import os

# QT stuff
from PySide6.QtWidgets import QApplication, QMainWindow, QMessageBox
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

        # Init variables
        self.radioList = []
        self.editing = False
        self.editingIndex = None
        self.editingRadio = None

        # Setup UI elements
        self.setupUI()

    # Connect UI elements to functions
    def setupUI(self):
        # Setup signals for menu items
        self.ui.aMenuQuit.triggered.connect(self.quit)      # Quit program

        # Setup signals for buttons
        self.ui.fAddRadio.clicked.connect(self.addNewRadio)

        # Setup signals for radio list
        self.ui.fEditRadio.clicked.connect(self.editRadio)
        self.ui.fRadioList.currentItemChanged.connect(self.selectRadio)

        # Setup signals for radio editing GUI
        self.ui.fEditRadioSigMode.currentTextChanged.connect(self.selectSignalMode)

        # Populate valid control modes
        for mode in Radio.controlModes:
            self.ui.fEditRadioCtrlMode.addItem(mode)

        # Populate valid signalling modes
        for mode in Radio.signallingModes:
            self.ui.fEditRadioSigMode.addItem(mode)

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

    # Update the list of radios from the radio list object 
    def updateRadioList(self):
        # clear list
        self.ui.fRadioList.clear()
        # update list
        for radio in self.radioList:
            print(radio.name)
            self.ui.fRadioList.addItem(radio.name)

    # Enable/disable radio edit dialog
    def enableRadioEdit(self, enabled):
        # status variable
        self.editing = enabled
        # enable edit boxes
        self.ui.fEditRadioName.setEnabled(enabled)
        self.ui.fEditRadioDesc.setEnabled(enabled)
        self.ui.fEditRadioCtrlMode.setEnabled(enabled)
        self.ui.fEditRadioCtrlPort.setEnabled(enabled)
        self.ui.fEditRadioPTT.setEnabled(enabled)
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
        self.radioList.append(newRadio)
        # Add the radio name to the list view
        self.ui.fRadioList.addItem(newRadio.name)

    # New radio in list selected
    def selectRadio(self):
        if self.ui.fRadioList.currentItem() != None:
            # get the currently selected item
            selectedName = self.ui.fRadioList.currentItem().text()
            # get the associated radio from the list
            for radio in self.radioList:
                if radio.name == selectedName:
                    break
            else:
                raise ValueError("Selected radio not present in radio list")
            # populate the radio info pane
            self.populateRadioFields(radio)
        else:
            self.populateRadioFields(None)

    # Edit radio button clicked
    def editRadio(self):
        # Start Editing
        if not self.editing:
            # get the selected radio
            radioName = self.ui.fRadioList.currentItem().text()
            print("Editing radio {}".format(radioName))
            # get the associated radio from the list
            for index, radio in enumerate(self.radioList):
                if radio.name == radioName:
                    break
            else:
                raise ValueError("Selected radio not present in radio list")
            # Update the currently editing variables
            self.editingIndex = index
            self.editingRadio = self.radioList[index]
            # enable editing
            self.enableRadioEdit(True)
        # Save edits
        else:
            # check if name already exists and isn't the one we're editing
            newName = self.ui.fEditRadioName.text()
            for index, radio in enumerate(self.radioList):
                if radio.name == newName and index != self.editingIndex:
                    self.showError("Radio name already exists!")
                    return
            # create a new radio object
            newRadio = Radio(
                name=newName,
                desc=self.ui.fEditRadioDesc.text(),
                ctrlMode=self.ui.fEditRadioCtrlMode.currentText(),
                ctrlPort=self.ui.fEditRadioCtrlPort.currentText(),
                pttDev=self.ui.fEditRadioPTT.currentText(),
                txDev=self.ui.fEditRadioTxAud.currentText(),
                rxDev=self.ui.fEditRadioRxAud.currentText(),
                signalMode=self.ui.fEditRadioSigMode.currentText(),
                signalId=self.ui.fEditRadioSigId.text()
            )
            # update the list with the new object
            self.radioList[self.editingIndex] = newRadio
            # reset the editing variables
            self.editingIndex = None
            self.editingRadio = None
            # update the list of radios
            self.updateRadioList()
            # disable editing
            self.enableRadioEdit(False)

    def selectSignalMode(self):
        if self.editing:
            if self.ui.fEditRadioSigMode.currentText() == "None":
                self.ui.fEditRadioSigId.setEnabled(False)
            else:
                self.ui.fEditRadioSigId.setEnabled(True)

    # populate radio info UI elements
    def populateRadioFields(self, radio):
        if radio != None:
            self.ui.fEditRadioName.setText(radio.name)
            self.ui.fEditRadioDesc.setText(radio.desc)
            self.ui.fEditRadioCtrlMode.setCurrentText(radio.ctrlMode)
            self.ui.fEditRadioCtrlPort.setCurrentText(radio.ctrlPort)
            self.ui.fEditRadioPTT.setCurrentText(radio.pttDev)
            self.ui.fEditRadioTxAud.setCurrentText(radio.txDev)
            self.ui.fEditRadioRxAud.setCurrentText(radio.rxDev)
            self.ui.fEditRadioSigMode.setCurrentText(radio.sigMode)
            self.ui.fEditRadioSigId.setText(radio.sigId)
        else:
            self.ui.fEditRadioName.setText("")
            self.ui.fEditRadioDesc.setText("")
            self.ui.fEditRadioCtrlMode.setCurrentText("")
            self.ui.fEditRadioCtrlPort.setCurrentText("")
            self.ui.fEditRadioPTT.setCurrentText("")
            self.ui.fEditRadioTxAud.setCurrentText("")
            self.ui.fEditRadioRxAud.setCurrentText("")
            self.ui.fEditRadioSigMode.setCurrentText("")
            self.ui.fEditRadioSigId.setText("")

    # Show error dialog
    def showError(self, message):
        msgBox = QMessageBox(QMessageBox.Critical)
        msgBox.setText(message)
        msgBox.exec()

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