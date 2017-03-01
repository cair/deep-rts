import zmq
from algorithms.proto.TestMessage_pb2 import TestMessage


port = "5555"
context = zmq.Context()
socket = context.socket(zmq.PAIR)
socket.connect("tcp://localhost:%s" % port)



while True:
    #msg = socket.recv()
    tMessage = TestMessage()

    tMessage.id = 1
    tMessage.query = "This is a message!"
    msg = tMessage.SerializeToString()

    socket.send(msg)


