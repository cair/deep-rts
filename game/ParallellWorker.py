
import os
from multiprocessing import Queue
from multiprocessing import Process
from threading import Thread

import time

from game import Config
from game.WarC2 import Game

STATUS = 0
GAMESTATE = 10


# Status enums (Which describes a proces which are idle/working
STATUS_IDLE = 0
STATUS_WORKING = 1


class Proc:

    def __init__(self, _id):
        self.id = _id
        self.i = None
        self.o = None
        self.status = None
        self.proc = None
        self.data = {
            'state': None,
            'last_update': None
        }



class ParallellWorker:

    def get_idle(self):
        idle_workers = [p for p in self.procs if p.status == STATUS_IDLE]
        if not idle_workers:
            return None

        return idle_workers.pop()

    def input_handler(self):
        while True:
            for proc in self.procs:
                in_q = proc.i
                in_q.put({
                    'type': STATUS,
                    'data': None
                })

            time.sleep(1)

    def output_handler(self):
        while True:
            for proc in self.procs:
                in_queue = proc.i
                out_queue = proc.o

                # Update last_updated
                proc.data['last_update'] = time.time()

                # Ignore processing if no items are present (Nothing to report)
                if out_queue.empty():
                    continue

                data = out_queue.get(False)

                if data['type'] == GAMESTATE:
                    proc.data['game'] = data

    def __init__(self, gui=True):

        # Create a process container
        self.procs = []

        # Create processing units
        for i in range(Config.MULTIPROCESSING_WORKERS):
            # Create input and output gate
            in_queue = Queue()
            out_queue = Queue()

            # Spawn the process inputting an id, and the io gates
            proc = Worker(i, in_queue, out_queue, gui)
            proc.start()

            # Create a entry in the process state dict
            proc = Proc(i)
            proc.i = in_queue
            proc.o = out_queue
            proc.status = STATUS_WORKING
            proc.proc = proc
            proc.data['process_id'] = i
            proc.data['last_update'] = time.time()
            self.procs.append(proc)

        # Output handler (Child Process => Main Process)
        output_handler = Thread(target=self.output_handler, args=())
        output_handler.start()

        # Input handler for testing (Main Process => Child Process)
        t_ = Thread(target=self.input_handler, args=())
        t_.start()


class Worker(Process):

    def on_event(self, tick):

        if not self.i.empty():
            data = self.i.get(False)

            if data['type'] == STATUS:
                """
                # Get game status (IE paused, running... etc
                """
                self.o.put({
                    'type': STATUS,
                    'data': None # TODO
                })

    def do_event(self, tick):
        """
        Do event is events going from Child Process => Main Process
        These are regular updates which updates the state which lies in the dictionary on Main process side
        :return: void
        """

        self.o.put({
            'type': GAMESTATE,
            'data': self.g.dump_state()
        })

    def __init__(self, _id, in_queue, out_queue, gui):
        super(Worker, self).__init__()
        self.i = in_queue
        self.o = out_queue
        self.id = _id
        self.gui = gui

        self.g = None

    def __del__(self):
        #print("Shutting down @ %s" % self.id)
        pass

    def run(self):
        #print("Started Worker %s" % self.id)

        self.g = Game(gui=self.gui)
        self.g.set_pause(False)
        self.g.clock.shedule(self.on_event, .1)
        self.g.clock.shedule(self.do_event, 3)
        self.g.loop()


