from xmlrpc.server import SimpleXMLRPCServer

from MultiplicationLargeNumbers import *


def main():
    server = SimpleXMLRPCServer(("localhost", 8000))
    server.register_function(difference)
    server.register_function(multiplication)
    server.register_function(summation)
    server.serve_forever()


if __name__ == '__main__':
    # main()