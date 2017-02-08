
import os
from multiprocessing import Queue
from multiprocessing import Process
from game import Config
from game.WarC2 import Game


class ParallellWorker:

    def __init__(self):
        print("Setting up paralellworker")
        request_queue = Queue()
        for i in range(Config.MULTIPROCESSING_WORKERS):
            Worker( request_queue ).start()

        # Sentinel objects to allow clean shutdown: 1 per worker.
        for i in range(4):
            request_queue.put( None )


class Worker(Process):
    def __init__(self, queue):
        super(Worker, self).__init__()
        self.queue = queue



    def __del__(self):
        print("Shutting down @ %s" % "DERP")

    def run(self):
        print("Started Worker")

        g = Game()
        g.set_pause(True)
        g.loop()

