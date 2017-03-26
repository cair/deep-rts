"""
Base class for all algorithms
"""


class Algorithm:

    def __init__(self, action):
        self.Action = action
        self.tick = 0

    def increment_tick(self):
        self.tick += 1

    def ready(self):
        raise Exception('Ready() is not implemented in your algorithm!')

    def compute(self):
        raise Exception('Compute() is not implemented in your algorithm!')


