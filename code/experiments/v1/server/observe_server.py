import time
from multiprocessing import Process


class ObserveServer(Process):
    def __init__(self, o_in_q, o_out_q, weight_queue, agent):
        Process.__init__(self)
        self.o_in_queue = o_in_q
        self.o_out_queue = o_out_q
        self.weight_queue = weight_queue
        self.agent = agent

    def run(self):
        print("Starting ObserveServer loop")
        while True:

            while not self.o_in_queue.empty():

                if not self.weight_queue.empty():
                    break

                sample = self.o_in_queue.get()
                x, y, errors = self.agent.observe_no_update(sample=sample)
                self.o_out_queue.put((errors, sample))

            while not self.weight_queue.empty():
                loss, generation, weights, memory = self.weight_queue.get()
                self.agent.brain.set_weights(weights)
                print("Updated ObserveServer weights (Gen: %s, Loss: %s)" % (generation, loss))

            time.sleep(.1)
