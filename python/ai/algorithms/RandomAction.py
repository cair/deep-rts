import random
from ai.algorithms.Algorithm import Algorithm


class RandomAction(Algorithm):

    def __init__(self, action):
        super().__init__(action)
        self.skip = 8 * 32
        self.next = 0

    def ready(self):
        if self.tick > self.next:
            self.next = self.tick * self.skip
            return True
        return False

    def compute(self):
        idle_workers = self.Action.idle_workers()

        if idle_workers:
            worker = idle_workers[0]
            possible_actions = self.Action.possible_actions(worker)
            action = random.choice(possible_actions)
            action[0](*action[1])


