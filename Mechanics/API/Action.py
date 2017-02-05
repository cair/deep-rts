from Mechanics.Constants import Map
from Mechanics.Unit.States import State
import numpy as np

from Mechanics.Unit import Unit


class Action:

    def __init__(self, player):
        self.player = player
        self.game = player.game

        self.edge_wood_tiles = self._edge_tiles(type=Map.WOOD)
        self.edge_gold_tiles = self._edge_tiles(type=Map.GOLD)

        self.environment_tiles = np.where(self.game.state.data['tile'] == Map.GRASS)
        self.environment_tiles = set(zip(*self.environment_tiles))

    def _edge_tiles(self, type=None):
        tiles = self.game.state.data['tile']

        # Environment tiles
        env_tiles = np.where(tiles == type)
        env_tiles = set(zip(*env_tiles))

        eligble_tiles = []
        for (x, y) in env_tiles:
            adjacent_tiles = self.game.Map.AdjacentMap.adjacent_walkable(x, y, 1)
            if adjacent_tiles:
                eligble_tiles.append((x, y))
        return eligble_tiles

    def workers(self):
        p_units = self.player.get_units()

        units = [x for x in  p_units if (x.state.type != State.Dead or x.state.type != State.Despawned or x.state.type != State.Spawning) and x.is_worker()]

        return units

    def idle_town_halls(self):
        return [x for x in self.player.get_units() if (
            x.state.type != State.Dead or
            x.state.type != State.Despawned or
            x.state.type != State.Spawning or
            x.state.type == State.Idle) and
            x.id == Unit.TOWN_HALL]

    def idle_units(self, type):
        return [x for x in self.player.get_units() if x.state.type == State.Idle and x.id == type]

    def idle_workers(self):
        return [x for x in self.player.get_units() if x.state.type == State.Idle and x.is_worker()]

    def idle_barracks(self):
        return [x for x in self.player.get_units() if x.state.type == State.Idle and x.id == Unit.BARRACKS]

    def idle_barracks(self):
        return [x for x in self.player.get_units() if x.state.type == State.Idle and x.id == Unit.BARRACKS]

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
        units = set([y for x in [x.unit_area() for x in self.game.units.values() if x.player != unit.player] for y in x])
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

    def harvest_gold(self):
        """
        Takes a random worker for gold harvesting
        :return: False if no worker exists, true if a worker was assigned harvesting
        """

        workers = self.workers()
        if not workers:
            return False

        actions = self.harvest_gold_actions(workers.pop())
        if not actions:
            return False

        action = actions.pop()
        action[0](*action[1])
        return True

    def harvest_lumber(self):
        """
        Takes a random worker for gold harvesting
        :return: False if no worker exists, true if a worker was assigned harvesting
        """

        workers = self.workers()
        if not workers:
            return False

        actions = self.harvest_wood_actions(workers.pop())
        if not actions:
            return False

        action = actions.pop()
        action[0](*action[1])
        return True

    def build_peasant(self):

        town_halls = self.idle_town_halls()
        if not town_halls:
            return False

        town_hall = town_halls.pop()
        town_hall.build(0)

        return True

    def build_footman(self):

        barracks = self.idle_barracks()

        if not barracks:
            return False

        barrack = barracks.pop()
        barrack.build(0)

    def worker_build(self, idx):

        workers = self.workers()
        if not workers:
            return False

        worker = workers.pop()
        return worker.build(idx)


    def attack(self, player, id):
        """
        Find random :id: unit and attack it with footman
        :param id:
        :return:
        """

        player_footmans = [unit for unit in self.game.units.values() if unit.player.id == player.id and unit.id == Unit.FOOTMAN]
        if not player_footmans:
            return False

        footman = player_footmans.pop()

        opponent_units = [unit for unit in self.game.units.values() if unit.player.id != player.id and unit.id == id]
        if not opponent_units:
            return True

        opp_unit = opponent_units.pop()

        footman.right_click(opp_unit.x, opp_unit.y)

        return True





    def possible_actions(self, unit):
        move_actions = [(unit.right_click, arguments) for arguments in self.move_actions(unit)]
        build_actions = self.build_actions(unit)
        harvest_actions = self.harvest_wood_actions(unit) + self.harvest_gold_actions(unit)
        attack_actions = self.attack_actions(unit)

        return harvest_actions + move_actions + build_actions + attack_actions