import random


class EventLoop:

    def __init__(self, algorithm, player):
        self.algorithm = algorithm

        player.Event.c_event = self.on_event
        # TODO add more events

    def on_event(self, event_type, data):

        if self.algorithm.ready():
            self.algorithm.compute()
        self.algorithm.increment_tick()

    def on_defeat(data):
        pass

    def on_victory(data):
        pass

