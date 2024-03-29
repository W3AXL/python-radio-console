# Colorized logs
from colorama import init, Fore, Back, Style

import time
from datetime import datetime

import inspect

class Logger:

    """-------------------------------------------------------------------------------
        Logging Print Functions
    -------------------------------------------------------------------------------"""

    def __init__(self, verbose=False, debug=False):
        # Save verbosity
        self.verbose = verbose
        self.debug = debug

    def initLogs(self):
        # Init colorama
        init()

    def setVerbose(self, verbose):
        self.verbose = verbose

    def setDebug(self, debug):
        self.debug = debug

    def logDebug(self, msg):
        if self.debug:
            callingFunction = inspect.currentframe().f_back.f_code.co_name
            timeString = datetime.now().strftime("%m/%d %H:%M:%S.%f")[:-3]
            print(Fore.LIGHTBLUE_EX + Style.DIM + "[{}] ({:^16}) DEBG: {}".format(timeString, callingFunction, str(msg)) + Style.RESET_ALL)

    def logVerbose(self, msg):
        if self.verbose:
            callingFunction = inspect.currentframe().f_back.f_code.co_name
            timeString = datetime.now().strftime("%m/%d %H:%M:%S.%f")[:-3]
            print(Fore.WHITE + Style.DIM + "[{}] ({:^16}) VERB: {}".format(timeString, callingFunction, str(msg)) + Style.RESET_ALL)

    def logInfo(self, msg):
        callingFunction = inspect.currentframe().f_back.f_code.co_name
        timeString = datetime.now().strftime("%m/%d %H:%M:%S.%f")[:-3]
        print(Fore.WHITE + "[{}] ({:^16}) INFO: {}".format(timeString, callingFunction, str(msg)) + Style.RESET_ALL)

    def logWarn(self, msg):
        callingFunction = inspect.currentframe().f_back.f_code.co_name
        timeString = datetime.now().strftime("%m/%d %H:%M:%S.%f")[:-3]
        print(Fore.YELLOW + "[{}] ({:^16}) WARN: {}".format(timeString, callingFunction, str(msg)) + Style.RESET_ALL)

    def logError(self, msg):
        callingFunction = inspect.currentframe().f_back.f_code.co_name
        timeString = datetime.now().strftime("%m/%d %H:%M:%S.%f")[:-3]
        print(Fore.RED + "[{}] ({:^16}) ERRR: {}".format(timeString, callingFunction, str(msg)) + Style.RESET_ALL)