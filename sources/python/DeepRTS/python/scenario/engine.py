from functools import partial
from DeepRTS import python
from DeepRTS.Engine import Player


class Scenario:

    def __init__(self, game: python.Game, *scenarios):
        self.game = game

        self.scenarios = [partial(scenario, self) for scenario in scenarios]

    def evaluate(self):
        values = [scenario() for scenario in self.scenarios]
        return all(values), values

    @staticmethod
    def _gold_collect(amount: int, player=0):
        def wrap(self: Scenario):
            p: Player = self.game.players[player]
            return p.statistic_gathered_gold >= amount

        return wrap

    @staticmethod
    def _lumber_collect(amount: int, player=0):
        def wrap(self: Scenario):
            p: Player = self.game.players[player]
            return p.statistic_gathered_lumber >= amount

        return wrap

    @staticmethod
    def _oil_collect(amount: int, player=0):
        def wrap(self: Scenario):
            p: Player = self.game.players[player]
            return p.statistic_gathered_oil >= amount

        return wrap

    @staticmethod
    def _food_consumption(amount: int, player=0):
        def wrap(self: Scenario):
            p: Player = self.game.players[player]
            return p.food_consumption >= amount

        return wrap

    @staticmethod
    def _food_count(amount: int, player=0):
        def wrap(self: Scenario):
            p: Player = self.game.players[player]
            return p.food >= amount

        return wrap

    @staticmethod
    def _damage_done(amount: int, player=0):
        def wrap(self: Scenario):
            p: Player = self.game.players[player]
            return p.statistic_damage_done >= amount

        return wrap

    @staticmethod
    def _damage_taken(amount: int, player=0):
        def wrap(self: Scenario):
            p: Player = self.game.players[player]
            return p.statistic_damage_taken >= amount

        return wrap

    @staticmethod
    def _units_created(amount: int, player=0):
        def wrap(self: Scenario):
            p: Player = self.game.players[player]
            return p.statistic_units_created >= amount

        return wrap

    @staticmethod
    def _num_footman(amount: int, player=0):
        def wrap(self: Scenario):
            p: Player = self.game.players[player]
            return p.num_footman >= amount

        return wrap

    @staticmethod
    def _num_peasant(amount: int, player=0):
        def wrap(self: Scenario):
            p: Player = self.game.players[player]
            return p.num_peasant >= amount

        return wrap

    @staticmethod
    def _num_archer(amount: int, player=0):
        def wrap(self: Scenario):
            p: Player = self.game.players[player]
            return p.num_archer >= amount

        return wrap

    @staticmethod
    def _num_farm(amount: int, player=0):
        def wrap(self: Scenario):
            p: Player = self.game.players[player]
            return p.num_farm >= amount

        return wrap

    @staticmethod
    def _num_barracks(amount: int, player=0):
        def wrap(self: Scenario):
            p: Player = self.game.players[player]
            return p.num_barrack >= amount

        return wrap

    @staticmethod
    def _num_town_hall(amount: int, player=0):
        def wrap(self: Scenario):
            p: Player = self.game.players[player]
            return p.num_town_hall >= amount

        return wrap

    def reset(self):
        self.game.reset()
        self.game.update()
        self.game.render()
        return self.game.get_state()

    def step(self, action, render="ai"):
        player: Player = self.game.selected_player # py::return_value_policy::reference
        player.do_action(action)

        self.game.update()
        self.game.render()
        if render == "human":
            self.game.view()

        s1 = self.game.get_state()
        t, tarr = self.evaluate()
        r = 1 if t else 0

        return s1, r, t, {}

    GOLD_COLLECT = _gold_collect
    OIL_COLLECT = _lumber_collect
    LUMBER_COLLECT = _oil_collect
    FOOD_CONSUMPTION = _food_consumption
    FOOD_COUNT = _food_count
    DAMAGE_DONE = _damage_done
    DAMAGE_TAKEN = _damage_taken
    UNITS_CREATED = _units_created

    NUM_FOOTMAN = _num_footman
    NUM_PEASANT = _num_peasant
    NUM_ARCHER = _num_archer

    NUM_FARM = _num_farm
    NUM_BARRACKS = _num_barracks
    NUM_TOWN_HALL = _num_town_hall
