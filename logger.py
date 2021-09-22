# Colorized logs
from colorama import init, Fore, Back, Style

import time

import inspect

class Logger:

    """-------------------------------------------------------------------------------
        Logging Print Functions
    -------------------------------------------------------------------------------"""

    def __init__(self, verbose=False):
        # Save verbosity
        self.verbose = verbose

    def initLogs(self):
        # Init colorama
        init()

    def setVerbose(self, verbose):
        self.verbose = verbose

    def logVerbose(self, msg):
        if self.verbose:
            callingFunction = inspect.currentframe().f_back.f_code.co_name
            timeString = time.strftime("%m/%d %H:%M:%S")
            print(Fore.WHITE + Style.DIM + "[{}] ({:^16}) VERB: {}".format(timeString, callingFunction, msg) + Style.RESET_ALL)

    def logInfo(self, msg):
        callingFunction = inspect.currentframe().f_back.f_code.co_name
        timeString = time.strftime("%m/%d %H:%M:%S")
        print(Fore.WHITE + "[{}] ({:^16}) INFO: {}".format(timeString, callingFunction, msg) + Style.RESET_ALL)

    def logWarn(self, msg):
        callingFunction = inspect.currentframe().f_back.f_code.co_name
        timeString = time.strftime("%m/%d %H:%M:%S")
        print(Fore.YELLOW + "[{}] ({:^16}) WARN: {}".format(timeString, callingFunction, msg) + Style.RESET_ALL)

    def logError(self, msg):
        callingFunction = inspect.currentframe().f_back.f_code.co_name
        timeString = time.strftime("%m/%d %H:%M:%S")
        print(Fore.RED + "[{}] ({:^16}) ERRR: {}".format(timeString, callingFunction, msg) + Style.RESET_ALL)