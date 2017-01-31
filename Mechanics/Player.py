import logging

import numpy as np

from Mechanics.API.Event import Event
from Mechanics.API.Interface import Interface
from Mechanics.Constants import Map as MapC, Config
from Mechanics.Constants import Race as RaceConstant
from Mechanics.Map import Map
from Mechanics.Unit.UnitManager import UnitManager


class Player:
    index = 1

    lumber = 800
    gold = 1200 * 100
    oil = 0
    food = 200
    consumed_food = 0

    def __init__(self, game, name="[NO-NAME]"):
        self.game = game
        self.Map = game.Map

        self.id = Player.index
        Player.index += 1
        self.defeated = False
        self.Event = Event(self)
        self.Interface = Interface(self, self.Event)
        self.Interface.start()

        self.name = name
        self.race = RaceConstant.HUMAN

        self.units = []
        self.vision = [x for x in self._base_vision()]
        #self.calculate_fow()

        logging.debug("Created player %s" % name)
        self.game.clock.shedule(self.check_defeat, 1.0)  # Regularly check for defeat

        self.spawn()
        self.Event.notify_start({
            'player_id': self.id,
            'worker': self.units[0]
        })

    def _base_vision(self):
        """
        Fog of war base.
        This is the inital 2d array which defines vision for walls and nothing else
        :return: base fow
        """
        if Config.NO_FOG:
            """ 100 % vision"""
            tiles = np.nonzero(np.ones((self.Map.height, self.Map.width), dtype=np.int))
            return list(zip(tiles[0], tiles[1]))

        else:
            tiles = np.nonzero(self.game.data['tile'] == MapC.WALL)
            base_vision = list(zip(tiles[0], tiles[1]))
            return base_vision

    def check_defeat(self, tick):
        has_units = True if self.units else False
        self.defeated = has_units

    def get_units(self):
        return [self.game.units[x] for x in self.units]

    def process(self, dt):

        for unit_id in self.units:
            self.game.units[unit_id].process(dt)

        if not Config.NO_FOG:
            self.calculate_fow()

    def spawn(self):
        # Get single spawn tile
        spawn_tile = self.Map.get_spawn_tile()

        # Create new unit
        unit = UnitManager.worker(self)      # 1. Create a worker
        unit.set_position(*spawn_tile)       # 2. Set worker position
        unit.add_to_game()                   # 3. Add worker to game
        unit.spawn()                         # 4. Spawn worker
        unit.state.transition()              # 5. Transition from Spawning to Idle state

        logging.debug("Player %s spawned at %s with 1 worker", self.name, spawn_tile)

        self.Event.notify(Event.Player_Spawn, data=self.index)

    def calculate_fow(self):
        self.vision = []

        for unit_id in self.units:
            unit = self.game.units[unit_id]
            if not unit.x:
                continue

            range_x = range(unit.x - 2 + unit.dimension, min(unit.x + 3 + unit.dimension, self.Map.height))
            range_y = range(unit.y - 2 + unit.dimension, min(unit.y + 3 + unit.dimension, self.Map.width))
            for x in range_x:
                for y in range_y:
                    self.vision.append((x, y))

    def right_click(self, x, y, unit_id):
        self.game.units[unit_id].right_click(x, y)

    def left_click(self, x, y, unit_id):
        pass













