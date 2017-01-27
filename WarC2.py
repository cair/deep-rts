import pygame
import sys

from Event import Event
from Mechanics.Map import Map
from Mechanics.Player import Player
from GUI import GUI
from Mechanics.Constants import Config


import numpy as np


class NoGUI:
    def __init__(self, game):
        self.game = game

    def draw(self, dt):
        pass

    def process(self):
        pass

    def caption(self, dt):
        print(
            ' '.join(('Loop=GameClock Tab:[TPS=%d MaxFPS=%d]',
                      'Runtime:[FPS=%d UPS=%d]')) % (
                self.game.clock.max_ups,
                self.game.clock.max_fps,
                self.game.clock.fps,
                self.game.clock.ups))


class Game:

    def __init__(self, map_name=None, players=None, clock=None):

        self.clock = clock
        self.map = Map(self, map_name, players)                  # Initialize Map
        self.units = {}                                    # All Units of the game
        self.unit_map = np.zeros((self.map.MAP_WIDTH, self.map.MAP_HEIGHT), dtype=np.int)  # Map of all units (uniq id)
        self.players = [Player(self, "Player %s" % x) for x in range(players)]

        self.gui = NoGUI(self) if not Config.HAS_GUI else GUI(self, self.players[0])


    def get_unit(self, x, y):
        return self.unit_map[x][y]

    def process(self, dt, frame):
        for p in self.players:
            p.process(dt)
        self.gui.process()

        Event.notify(Event.New_State, frame)

    def draw(self, dt):
        self.gui.draw(dt)

