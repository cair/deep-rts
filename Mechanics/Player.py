import logging

import numpy as np

from Mechanics.API.Event import Event
from Mechanics.API.Interface import Interface
from Mechanics.Constants import Map as MapC, Config
from Mechanics.Constants import Race as RaceConstant
from Mechanics.Unit.UnitManager import UnitManager

class PlayerState:
    def __init__(self, game, name):
        self.lumber = 1000
        self.gold = 2000
        self.oil = 1000
        self.food = 1
        self.consumed_food = 0
        self.name = name

        self.plogic = PlayerLogic(game)

        self.defeated = False
        self._surrender = False

        self.statistics = {
            'kill_count': 0,
            'death_count': 0,
            'unit_count': 0,
            'gold_count': 0,
            'lumber_count': 0,
            'oil_count': 0
        }

        self.units = []
        self.vision_map = [x for x in self.plogic._base_vision()]
        self.vision_memory = np.zeros((self.plogic.Map.height, self.plogic.Map.width, 4), dtype=np.int) # 4 Layers of memory
        self.interaction_map = set()


class PlayerLogic:
    index = 1

    def __init__(self, game):
        self.id = PlayerLogic.index
        PlayerLogic.index += 1
        self.race = RaceConstant.HUMAN

        self.game = game
        self.Map = game.Map

        self.Event = Event(self)
        self.Interface = Interface(self, self.Event)
        self.Interface.start()

        self.reset()

    def reset(self):
        self.state = PlayerState()
        logging.debug("Created player %s" % self.name)


        self.spawn()



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

    def surrender(self):
        self._surrender = True

    def get_units(self):
        return [self.game.units[x] for x in self.units]

    def process(self, dt):

        for unit_id in self.units:
            self.game.units[unit_id].process(dt)

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













