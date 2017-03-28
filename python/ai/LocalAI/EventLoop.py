import random


class EventLoop:

    def __init__(self, algorithm, player):
        self.algorithm = algorithm

        self.terminal = False

        player.Event.c_event = self.on_event
        player.Event.c_defeat = self.on_defeat
        player.Event.c_victory = self.on_victory
        # TODO add more events

    def on_event(self, event_type, data, tick):

        if self.algorithm.ready() and not self.terminal:
            self.algorithm.compute()
        self.algorithm.increment_tick()

    def on_defeat(self, data, tick):
        print("Terminal DEF", tick)
        self.terminal = True

    def on_victory(self, data, tick):
        print("Terminal VIC", tick)
        self.terminal = True

