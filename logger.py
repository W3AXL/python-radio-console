# Colorized logs
from colorama import init, Fore, Back, Style

import time

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
            timeString = time.strftime("%Y%m%d %H:%M:%S")
            print(Fore.WHITE + Style.DIM + "[{}] VERB: {}".format(timeString, msg) + Style.RESET_ALL)

    def logInfo(self, msg):
        timeString = time.strftime("%Y%m%d %H:%M:%S")
        print(Fore.WHITE + "[{}] INFO: {}".format(timeString, msg) + Style.RESET_ALL)

    def logWarn(self, msg):
        timeString = time.strftime("%Y%m%d %H:%M:%S")
        print(Fore.YELLOW + "[{}]WARN: {}".format(timeString, msg) + Style.RESET_ALL)

    def logError(self, msg):
        timeString = time.strftime("%Y%m%d %H:%M:%S")
        print(Fore.RED + "[{}] ERROR: {}".format(timeString, msg) + Style.RESET_ALL)