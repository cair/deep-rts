import zmq
import time
import sys
from threading import Thread
from algorithms.proto.GameMessage_pb2 import GameMessage

def worker(context, port, isIn):
    worker = context.socket(zmq.PAIR)
    worker.connect("tcp://localhost:" + str(port))

    perf_counter = 0
    perf_next = time.time() + 1

    while True:

        if isIn:
            # Receieve only
            M = worker.recv()
            game_messages = GameMessage()
            game_messages.ParseFromString(M);




            perf_counter += 1
        else:
            # Send only
            worker.send_string("Hello")
            perf_counter +=1

        if time.time() > perf_next:
            type = "INPUT" if isIn else "OUTPUT"
            print(type + ": " + str(perf_counter))
            perf_counter = 0
            perf_next = time.time() + 1

context = zmq.Context()
Thread(target=worker, args=(context, 5000, False)).start()
Thread(target=worker, args=(context, 5001, True)).start()
