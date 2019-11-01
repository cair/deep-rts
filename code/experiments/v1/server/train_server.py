import time
import pickle
from .database import *
from multiprocessing import Process


class TrainServer(Process):
    ANNOUNCE_EVERY = 50
    SAVE_EVERY = 1000
    MEMORY_INSERT_THRESHOLD = 5000

    def __init__(self, agent, o_in_q, w_queues):
        Process.__init__(self)
        self.agent = agent
        self.processed_observation_queue = o_in_q
        self.weight_out_queues = w_queues

    def add_to_w_queue(self):
        for w_out_q in self.weight_out_queues:
            w_out_q.put((
                self.agent.brain.loss,
                self.agent.generation,
                self.agent.brain.get_weights(),
                self.agent.memory
            ))

    def run(self):
        # Load latest generation of weights
        try:
            weights = Weights.select().order_by(Weights.id.desc()).get()
            self.agent.brain.set_weights(pickle.loads(weights.data))
            self.agent.generation = weights.generation
        except Exception as e:
            print("Could not find existing weights, using initial")

        # Load all existing experiences
        try:
            memory = Memory.select().order_by(Memory.id.desc()).get()
            self.agent.memory = pickle.loads(memory.data)
        except Exception as e:
            print(e)
            print("Could not find existing experiences")

        self.add_to_w_queue()

        print("Starting TrainServer loop")
        while True:
            max_mem_process = 0
            while not self.processed_observation_queue.empty() or max_mem_process > TrainServer.MEMORY_INSERT_THRESHOLD:
                errors, sample = self.processed_observation_queue.get()
                self.agent.memory.add(errors[0], sample)
                self.agent.steps += 1
                max_mem_process += 1

                if max_mem_process > TrainServer.MEMORY_INSERT_THRESHOLD:
                    print("Memory insertion seems is throttled to preserve training. (Qsize: %s)" % self.processed_observation_queue.qsize())

            if self.agent.steps > self.agent.BATCH_SIZE:
                self.agent.replay()
                print("Train generation %s " % self.agent.generation)
                if self.agent.generation % TrainServer.ANNOUNCE_EVERY == 0:
                    print("Announcing at %s " % self.agent.generation)
                    self.add_to_w_queue()

                if self.agent.generation % TrainServer.SAVE_EVERY == 0:
                    print("Saving at %s " % self.agent.generation)
                    # Save Weight and memory
                    db_w = Weights.create(data=pickle.dumps(self.agent.brain.get_weights()), generation=self.agent.generation)
                    db_w.save()

                    db_mem = Memory.create(data=pickle.dumps(self.agent.memory), generation=self.agent.generation)
                    db_mem.save()


            else:
                time.sleep(.1)
