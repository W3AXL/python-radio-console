# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'serverwindow.ui'
##
## Created by: Qt User Interface Compiler version 6.1.2
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import *  # type: ignore
from PySide6.QtGui import *  # type: ignore
from PySide6.QtWidgets import *  # type: ignore


class Ui_ServerWindow(object):
    def setupUi(self, ServerWindow):
        if not ServerWindow.objectName():
            ServerWindow.setObjectName(u"ServerWindow")
        ServerWindow.resize(800, 440)
        sizePolicy = QSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(ServerWindow.sizePolicy().hasHeightForWidth())
        ServerWindow.setSizePolicy(sizePolicy)
        ServerWindow.setMinimumSize(QSize(800, 440))
        ServerWindow.setMaximumSize(QSize(800, 440))
        self.aMenuSaveConfig = QAction(ServerWindow)
        self.aMenuSaveConfig.setObjectName(u"aMenuSaveConfig")
        self.aMenuLoadConfig = QAction(ServerWindow)
        self.aMenuLoadConfig.setObjectName(u"aMenuLoadConfig")
        self.aMenuQuit = QAction(ServerWindow)
        self.aMenuQuit.setObjectName(u"aMenuQuit")
        self.actionAbout = QAction(ServerWindow)
        self.actionAbout.setObjectName(u"actionAbout")
        self.centralwidget = QWidget(ServerWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.grpRadioList = QGroupBox(self.centralwidget)
        self.grpRadioList.setObjectName(u"grpRadioList")
        self.grpRadioList.setGeometry(QRect(10, 10, 371, 371))
        self.fRadioListView = QListView(self.grpRadioList)
        self.fRadioListView.setObjectName(u"fRadioListView")
        self.fRadioListView.setGeometry(QRect(9, 30, 351, 281))
        self.fAddRadio = QPushButton(self.grpRadioList)
        self.fAddRadio.setObjectName(u"fAddRadio")
        self.fAddRadio.setGeometry(QRect(10, 320, 111, 41))
        self.fEditRadio = QPushButton(self.grpRadioList)
        self.fEditRadio.setObjectName(u"fEditRadio")
        self.fEditRadio.setGeometry(QRect(130, 320, 111, 41))
        self.fDeleteRadio = QPushButton(self.grpRadioList)
        self.fDeleteRadio.setObjectName(u"fDeleteRadio")
        self.fDeleteRadio.setGeometry(QRect(250, 320, 111, 41))
        self.grpRadioSettings = QGroupBox(self.centralwidget)
        self.grpRadioSettings.setObjectName(u"grpRadioSettings")
        self.grpRadioSettings.setGeometry(QRect(389, 10, 401, 371))
        self.gridLayoutWidget = QWidget(self.grpRadioSettings)
        self.gridLayoutWidget.setObjectName(u"gridLayoutWidget")
        self.gridLayoutWidget.setGeometry(QRect(9, 29, 381, 281))
        self.grdRadioSettings = QGridLayout(self.gridLayoutWidget)
        self.grdRadioSettings.setObjectName(u"grdRadioSettings")
        self.grdRadioSettings.setHorizontalSpacing(10)
        self.grdRadioSettings.setVerticalSpacing(2)
        self.grdRadioSettings.setContentsMargins(10, 5, 10, 5)
        self.lblRadioPTT = QLabel(self.gridLayoutWidget)
        self.lblRadioPTT.setObjectName(u"lblRadioPTT")
        self.lblRadioPTT.setAlignment(Qt.AlignRight|Qt.AlignTrailing|Qt.AlignVCenter)

        self.grdRadioSettings.addWidget(self.lblRadioPTT, 4, 0, 1, 1)

        self.fEditRadioRxAud = QComboBox(self.gridLayoutWidget)
        self.fEditRadioRxAud.setObjectName(u"fEditRadioRxAud")
        self.fEditRadioRxAud.setEnabled(False)

        self.grdRadioSettings.addWidget(self.fEditRadioRxAud, 6, 1, 1, 1)

        self.lblRadioSigMode = QLabel(self.gridLayoutWidget)
        self.lblRadioSigMode.setObjectName(u"lblRadioSigMode")
        self.lblRadioSigMode.setAlignment(Qt.AlignRight|Qt.AlignTrailing|Qt.AlignVCenter)

        self.grdRadioSettings.addWidget(self.lblRadioSigMode, 7, 0, 1, 1)

        self.lblRadioSigId = QLabel(self.gridLayoutWidget)
        self.lblRadioSigId.setObjectName(u"lblRadioSigId")
        self.lblRadioSigId.setAlignment(Qt.AlignRight|Qt.AlignTrailing|Qt.AlignVCenter)

        self.grdRadioSettings.addWidget(self.lblRadioSigId, 8, 0, 1, 1)

        self.fEditRadioTxAud = QComboBox(self.gridLayoutWidget)
        self.fEditRadioTxAud.setObjectName(u"fEditRadioTxAud")
        self.fEditRadioTxAud.setEnabled(False)

        self.grdRadioSettings.addWidget(self.fEditRadioTxAud, 5, 1, 1, 1)

        self.lblRadioDesc = QLabel(self.gridLayoutWidget)
        self.lblRadioDesc.setObjectName(u"lblRadioDesc")
        self.lblRadioDesc.setAlignment(Qt.AlignRight|Qt.AlignTrailing|Qt.AlignVCenter)

        self.grdRadioSettings.addWidget(self.lblRadioDesc, 1, 0, 1, 1)

        self.fEditRadioCtrlPort = QComboBox(self.gridLayoutWidget)
        self.fEditRadioCtrlPort.setObjectName(u"fEditRadioCtrlPort")
        self.fEditRadioCtrlPort.setEnabled(False)

        self.grdRadioSettings.addWidget(self.fEditRadioCtrlPort, 3, 1, 1, 1)

        self.fEditRadioSigMode = QComboBox(self.gridLayoutWidget)
        self.fEditRadioSigMode.addItem("")
        self.fEditRadioSigMode.addItem("")
        self.fEditRadioSigMode.addItem("")
        self.fEditRadioSigMode.addItem("")
        self.fEditRadioSigMode.addItem("")
        self.fEditRadioSigMode.setObjectName(u"fEditRadioSigMode")
        self.fEditRadioSigMode.setEnabled(False)

        self.grdRadioSettings.addWidget(self.fEditRadioSigMode, 7, 1, 1, 1)

        self.lblRadioTxAud = QLabel(self.gridLayoutWidget)
        self.lblRadioTxAud.setObjectName(u"lblRadioTxAud")
        self.lblRadioTxAud.setAlignment(Qt.AlignRight|Qt.AlignTrailing|Qt.AlignVCenter)

        self.grdRadioSettings.addWidget(self.lblRadioTxAud, 5, 0, 1, 1)

        self.lblRadioRxAud = QLabel(self.gridLayoutWidget)
        self.lblRadioRxAud.setObjectName(u"lblRadioRxAud")
        self.lblRadioRxAud.setAlignment(Qt.AlignRight|Qt.AlignTrailing|Qt.AlignVCenter)

        self.grdRadioSettings.addWidget(self.lblRadioRxAud, 6, 0, 1, 1)

        self.fEditRadioName = QLineEdit(self.gridLayoutWidget)
        self.fEditRadioName.setObjectName(u"fEditRadioName")
        self.fEditRadioName.setEnabled(False)

        self.grdRadioSettings.addWidget(self.fEditRadioName, 0, 1, 1, 1)

        self.fEditRadioSigId = QLineEdit(self.gridLayoutWidget)
        self.fEditRadioSigId.setObjectName(u"fEditRadioSigId")
        self.fEditRadioSigId.setEnabled(False)
        self.fEditRadioSigId.setMaxLength(8)

        self.grdRadioSettings.addWidget(self.fEditRadioSigId, 8, 1, 1, 1)

        self.fEditRadioDesc = QLineEdit(self.gridLayoutWidget)
        self.fEditRadioDesc.setObjectName(u"fEditRadioDesc")
        self.fEditRadioDesc.setEnabled(False)

        self.grdRadioSettings.addWidget(self.fEditRadioDesc, 1, 1, 1, 1)

        self.lblRadioName = QLabel(self.gridLayoutWidget)
        self.lblRadioName.setObjectName(u"lblRadioName")
        self.lblRadioName.setAlignment(Qt.AlignRight|Qt.AlignTrailing|Qt.AlignVCenter)

        self.grdRadioSettings.addWidget(self.lblRadioName, 0, 0, 1, 1)

        self.lblRadioCtrlPort = QLabel(self.gridLayoutWidget)
        self.lblRadioCtrlPort.setObjectName(u"lblRadioCtrlPort")
        self.lblRadioCtrlPort.setAlignment(Qt.AlignRight|Qt.AlignTrailing|Qt.AlignVCenter)

        self.grdRadioSettings.addWidget(self.lblRadioCtrlPort, 3, 0, 1, 1)

        self.fEditRadioPTT = QComboBox(self.gridLayoutWidget)
        self.fEditRadioPTT.setObjectName(u"fEditRadioPTT")
        self.fEditRadioPTT.setEnabled(False)

        self.grdRadioSettings.addWidget(self.fEditRadioPTT, 4, 1, 1, 1)

        self.lblRadioCtrlMode = QLabel(self.gridLayoutWidget)
        self.lblRadioCtrlMode.setObjectName(u"lblRadioCtrlMode")
        self.lblRadioCtrlMode.setAlignment(Qt.AlignRight|Qt.AlignTrailing|Qt.AlignVCenter)

        self.grdRadioSettings.addWidget(self.lblRadioCtrlMode, 2, 0, 1, 1)

        self.fEditRadioCtrlMode = QComboBox(self.gridLayoutWidget)
        self.fEditRadioCtrlMode.setObjectName(u"fEditRadioCtrlMode")
        self.fEditRadioCtrlMode.setEnabled(False)

        self.grdRadioSettings.addWidget(self.fEditRadioCtrlMode, 2, 1, 1, 1)

        self.fConnectRadio = QPushButton(self.grpRadioSettings)
        self.fConnectRadio.setObjectName(u"fConnectRadio")
        self.fConnectRadio.setEnabled(False)
        self.fConnectRadio.setGeometry(QRect(10, 320, 111, 41))
        self.fDisconnectRadio = QPushButton(self.grpRadioSettings)
        self.fDisconnectRadio.setObjectName(u"fDisconnectRadio")
        self.fDisconnectRadio.setEnabled(False)
        self.fDisconnectRadio.setGeometry(QRect(130, 320, 111, 41))
        ServerWindow.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(ServerWindow)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 800, 22))
        self.menu_File = QMenu(self.menubar)
        self.menu_File.setObjectName(u"menu_File")
        self.menu_Help = QMenu(self.menubar)
        self.menu_Help.setObjectName(u"menu_Help")
        ServerWindow.setMenuBar(self.menubar)
        self.statusbar = QStatusBar(ServerWindow)
        self.statusbar.setObjectName(u"statusbar")
        ServerWindow.setStatusBar(self.statusbar)

        self.menubar.addAction(self.menu_File.menuAction())
        self.menubar.addAction(self.menu_Help.menuAction())
        self.menu_File.addAction(self.aMenuSaveConfig)
        self.menu_File.addAction(self.aMenuLoadConfig)
        self.menu_File.addSeparator()
        self.menu_File.addAction(self.aMenuQuit)
        self.menu_Help.addAction(self.actionAbout)

        self.retranslateUi(ServerWindow)

        QMetaObject.connectSlotsByName(ServerWindow)
    # setupUi

    def retranslateUi(self, ServerWindow):
        ServerWindow.setWindowTitle(QCoreApplication.translate("ServerWindow", u"Radio Console Server", None))
        self.aMenuSaveConfig.setText(QCoreApplication.translate("ServerWindow", u"&Save Config", None))
        self.aMenuLoadConfig.setText(QCoreApplication.translate("ServerWindow", u"&Open Config", None))
        self.aMenuQuit.setText(QCoreApplication.translate("ServerWindow", u"&Quit", None))
        self.actionAbout.setText(QCoreApplication.translate("ServerWindow", u"About", None))
        self.grpRadioList.setTitle(QCoreApplication.translate("ServerWindow", u"Radio List", None))
        self.fAddRadio.setText(QCoreApplication.translate("ServerWindow", u"Add Radio", None))
        self.fEditRadio.setText(QCoreApplication.translate("ServerWindow", u"Edit Radio", None))
        self.fDeleteRadio.setText(QCoreApplication.translate("ServerWindow", u"Delete", None))
        self.grpRadioSettings.setTitle(QCoreApplication.translate("ServerWindow", u"Radio Settings", None))
        self.lblRadioPTT.setText(QCoreApplication.translate("ServerWindow", u"PTT Device", None))
        self.lblRadioSigMode.setText(QCoreApplication.translate("ServerWindow", u"Signalling", None))
        self.lblRadioSigId.setText(QCoreApplication.translate("ServerWindow", u"Signalling ID", None))
        self.lblRadioDesc.setText(QCoreApplication.translate("ServerWindow", u"Description", None))
        self.fEditRadioSigMode.setItemText(0, QCoreApplication.translate("ServerWindow", u"None", None))
        self.fEditRadioSigMode.setItemText(1, QCoreApplication.translate("ServerWindow", u"MDC1200", None))
        self.fEditRadioSigMode.setItemText(2, QCoreApplication.translate("ServerWindow", u"DTMF", None))
        self.fEditRadioSigMode.setItemText(3, QCoreApplication.translate("ServerWindow", u"Single-tone", None))
        self.fEditRadioSigMode.setItemText(4, QCoreApplication.translate("ServerWindow", u"Two-tone", None))

        self.lblRadioTxAud.setText(QCoreApplication.translate("ServerWindow", u"Transmit Audio", None))
        self.lblRadioRxAud.setText(QCoreApplication.translate("ServerWindow", u"Receive Audio", None))
        self.lblRadioName.setText(QCoreApplication.translate("ServerWindow", u"Name", None))
        self.lblRadioCtrlPort.setText(QCoreApplication.translate("ServerWindow", u"Control Port", None))
        self.lblRadioCtrlMode.setText(QCoreApplication.translate("ServerWindow", u"Control Mode", None))
        self.fConnectRadio.setText(QCoreApplication.translate("ServerWindow", u"Connect", None))
        self.fDisconnectRadio.setText(QCoreApplication.translate("ServerWindow", u"Disconnect", None))
        self.menu_File.setTitle(QCoreApplication.translate("ServerWindow", u"&File", None))
        self.menu_Help.setTitle(QCoreApplication.translate("ServerWindow", u"&Help", None))
    # retranslateUi

