import numpy as np
from game.const import State
from game.const import Map
from game.const import Unit



class Action:

    def __init__(self, player):
        self.player = player
        self.game = player.game

        self.edge_wood_tiles = self._edge_tiles(type=Map.WOOD)
        self.edge_gold_tiles = self._edge_tiles(type=Map.GOLD)

        self.environment_tiles = np.where(self.game.Map.tiles == Map.GRASS)
        self.environment_tiles = set(zip(*self.environment_tiles))

    def _edge_tiles(self, type=None):
        tiles = self.game.Map.tiles

        # Environment tiles
        env_tiles = np.where(tiles == type)
        env_tiles = set(zip(*env_tiles))

        eligble_tiles = []
        for (x, y) in env_tiles:
            adjacent_tiles = self.game.AdjacentMap.adjacent_walkable(self.game, x, y, 1)
            if adjacent_tiles:
                eligble_tiles.append((x, y))
        return eligble_tiles

    def workers(self):
        return [x for x in self.player.get_units() if (type(x.state) != State.Dead or type(x.state) != State.Despawned or type(x.state) != State.Spawning) and x.is_worker()]

    def idle_units(self, t):
        return [x for x in self.player.get_units() if type(x.state) == State.Idle and x.id == t]

    def idle_workers(self):
        return [x for x in self.player.get_units() if type(x.state) == State.Idle and x.is_worker()]

    def idle_barracks(self):
        return [x for x in self.player.get_units() if type(x.state) == State.Idle and x.id == Unit.BARRACKS]

    def idle_worker(self):
        workers = self.idle_workers()
        if workers:
            return workers.pop()
        else:
            return None

    def move_actions(self, unit):
        """
        Compiles a list of all possible places this unit can move
        :return:
        """
        # TODO Optimize
        #print(unit.state.x, unit.state, )
        units = set([y for x in [x.unit_area() for x in self.game.units.values()] for y in x])
        return self.environment_tiles - units

    def build_actions(self, unit):
        """
        Compiles a list of all possibles building this unit can build
        :return: List of possible builds at current position
        """
        possible_buildings = [(unit.build, [idx]) for (idx, val) in enumerate(unit.buildable) if unit.can_build_here(val)]
        return possible_buildings

    def harvest_wood_actions(self, unit):
        return [(unit.right_click, x) for x in self.edge_wood_tiles]

    def harvest_gold_actions(self, unit):
        return [(unit.right_click, x) for x in self.edge_gold_tiles]

    def attack_actions(self, unit):
        return [(unit.right_click, x) for x in list(zip(*np.where(self.game.data['unit'] != Unit.NONE)))]

    def possible_actions(self, unit):
        move_actions = [(unit.right_click, arguments) for arguments in self.move_actions(unit)]
        build_actions = self.build_actions(unit)
        harvest_actions = self.harvest_wood_actions(unit) + self.harvest_gold_actions(unit)
        #attack_actions = self.attack_actions(unit)

        return harvest_actions + move_actions + build_actions #+ attack_actions