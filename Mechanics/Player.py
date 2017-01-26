import numpy as np
import logging
import random

from Mechanics.Unit.UnitManager import UnitManager
from Mechanics.Constants import Race as RaceConstant
from Mechanics.Constants import Map as MapC
from Mechanics.Constants import Unit as UnitC

class Player:

    def __init__(self, game, name="[NO-NAME]"):
        self.name = name
        self.race = RaceConstant.HUMAN
        self.game = game
        self.unit_manager = UnitManager(self.race, self)
        self.lumber = 1200 * 5
        self.gold = 800 * 5
        self.oil = 0
        self.food = 2
        self.consumed_food = 0
        self.units = []
        self.fow = self.calculate_fow()

        logging.debug("Created player %s" % (name))

        self.spawn()

    def process(self, dt):

        for unit in self.units:
            unit.process(dt)

        self.calculate_fow

    def spawn(self):
        """
        Spawns the player randomly on the map
        A player starts with 1 worker
        :return:
        """

        available_tiles = self.game.map.spawn_points
        tile = available_tiles.pop(0)

        unit = self.unit_manager.worker()
        unit.set_position(*tile)
        unit.spawn()

        #self.add_unit(unit)

        logging.debug("Player %s spawned at %s with 1 worker", self.name, tile)

    def calculate_fow(self):
        fow = np.zeros((self.game.map.MAP_WIDTH, self.game.map.MAP_HEIGHT), dtype=np.int)
        for unit in self.units:

            # non spawned units
            if not unit.x or unit.y:
                continue

            range_x = range(unit.x - 2, min(unit.x + 3, self.game.map.MAP_WIDTH))
            range_y = range(unit.y - 2, min(unit.y + 3, self.game.map.MAP_HEIGHT))
            for x in range_x:
                for y in range_y:
                    self.fow[x][y] = 1

        walls = np.nonzero(self.game.map.tiles == MapC.WALL)
        for x, y in zip(walls[0], walls[1]):
            fow[x][y] = 1
        return fow













