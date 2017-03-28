import logging

from game import Constants
from game.unit.UnitManager import UnitManager
from game import Config
from collections import deque

import numpy as np


class Player:

    def __init__(self, game, id):
        self.id = id
        self.game = game
        self.unitManager = UnitManager
        self.name = "Player %s" % id

        self.player_color = (255, 255, 0) # TODO color

        self.action_statistics = np.zeros(20)

        self.faction = Constants.Race.Human
        self.gold = 1500
        self.lumber = 750
        self.oil = 0
        self.foodConsumption = 0
        self.food = 1
        self.defeated = False

        self.statGoldGather = 0
        self.statLumberGather = 0
        self.statOilGather = 0
        self.statUnitDamageDone = 0
        self.statUnitDamageTaken = 0
        self.statUnitBuilt = 0
        self.statUnitMilitary = 0

        self.algorithm = None
        self.apm = 0
        self.apm_counter = 0

        self.action_queue = deque()

        self.unit_indexes = []
        self.targeted_unit_id = -1

        logging.debug("Created player %s" % self.name)

    def spawn(self, spawn_tile):

        if self.faction == Constants.Race.Human:
            unit = self.add_unit(Constants.Unit.Peasant)
        elif self.faction == Constants.Race.Orc:
            unit = self.add_unit(Constants.Unit.Peon)
        else:
            assert False  # Should NEVER happen

        unit.spawn(spawn_tile, unit.spawn_duration)

        self.targeted_unit_id = unit.id

        return unit

    def add_unit(self, unit_type):

        unit = UnitManager.construct_unit(unit_type, self)
        self.unit_indexes.append(unit.id)
        return unit

    def reset(self):
       pass

    def update(self):

        # No actions, no update
        if not self.action_queue:
            return

        # Pop action
        action_id = self.action_queue.pop()

        # No units to perform action on
        if not self.unit_indexes:
            return

        if self.targeted_unit_id == -1 and action_id != Action.NextUnit and action_id != Action.PreviousUnit:
            return
        
        targeted_unit = self.targeted_unit()

        if action_id == Action.NextUnit:
            self.nextUnit()
        elif action_id == Action.PreviousUnit:
            self.previousUnit()
        elif action_id == Action.MoveUpRight:
            targeted_unit.tryMove(-1, 1);
        
        elif action_id == Action.MoveUpLeft:
            targeted_unit.tryMove(-1, -1);
        
        elif action_id == Action.MoveDownRight:
            targeted_unit.tryMove(1, 1);
        
        elif action_id == Action.MoveDownLeft:
            targeted_unit.tryMove(1, -1);
        
        elif action_id == Action.MoveUp:
            targeted_unit.tryMove(0, -1);
        
        elif action_id == Action.MoveDown:
            targeted_unit.tryMove(0, 1);
        
        elif action_id == Action.MoveLeft:
            targeted_unit.tryMove(-1, 0);
        
        elif action_id == Action.MoveRight:
            targeted_unit.tryMove(1, 0);
        
        elif action_id == Action.Attack:
            targeted_unit.tryAttack();
        
        elif action_id == Action.Harvest:
            targeted_unit.tryHarvest();
        
        elif action_id == Action.Build0:
            targeted_unit.build(0);
        
        elif action_id == Action.Build1:
            targeted_unit.build(1);
        
        elif action_id == Action.Build2:
            targeted_unit.build(2);
        
        elif action_id == Action.NoAction:
            pass
        else:
            assert False
            pass

    def targeted_unit(self):
        if self.targeted_unit_id is -1:
            return None

        return self.game.units[self.targeted_unit_id]









