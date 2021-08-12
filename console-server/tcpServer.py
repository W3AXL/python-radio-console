import socketserver

class serverTCPHandler(socketserver.StreamRequestHandler):
    """
    Main hander for data sent to the server from the client
    """

    def handle(self):
        """Handle lines received from the server
        """
        self.line = self.rfile.readline().strip()
        print("{} wrote:".format(self.client_address[0]))
        print(self.line)