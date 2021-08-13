from enum import Enum

class RadioState(Enum):
    """Radio status enum
    """
    # Standard connection states
    Disconnected = 0
    Idle = 1
    Receiving = 2
    Transmitting = 3
    # Error states
    ConnectError = 10
    TransmitError = 11
    ReceiveError = 12
    UnknownError = 20