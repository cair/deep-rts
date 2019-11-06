from functools import partial
import gym
import numpy as np
from DeepRTS.Engine import Constants


class Scenario(gym.Env):

    def __init__(self, game, *scenarios):
        self.game = game

        self.scenarios = [partial(scenario, self) for scenario in scenarios]

        self.action_space = gym.spaces.Discrete(Constants.action_max)
        self.observation_space = gym.spaces.Box(0, 255, shape=game.get_state().shape, dtype=np.float32)

    def evaluate(self):
        values = [scenario() for scenario in self.scenarios]
        return all(values), values

    @staticmethod
    def _gold_collect(amount, player=0):
        def wrap(self):
            p = self.game.players[player]
            return p.statistic_gathered_gold >= amount

        return wrap

    @staticmethod
    def _lumber_collect(amount, player=0):
        def wrap(self):
            p = self.game.players[player]
            return p.statistic_gathered_lumber >= amount

        return wrap

    @staticmethod
    def _oil_collect(amount, player=0):
        def wrap(self):
            p = self.game.players[player]
            return p.statistic_gathered_oil >= amount

        return wrap

    @staticmethod
    def _food_consumption(amount, player=0):
        def wrap(self):
            p = self.game.players[player]
            return p.food_consumption >= amount

        return wrap

    @staticmethod
    def _food_count(amount, player=0):
        def wrap(self):
            p = self.game.players[player]
            return p.food >= amount

        return wrap

    @staticmethod
    def _damage_done(amount, player=0):
        def wrap(self):
            p = self.game.players[player]
            return p.statistic_damage_done >= amount

        return wrap

    @staticmethod
    def _damage_taken(amount, player=0):
        def wrap(self):
            p = self.game.players[player]
            return p.statistic_damage_taken >= amount

        return wrap

    @staticmethod
    def _units_created(amount, player=0):
        def wrap(self):
            p = self.game.players[player]
            return p.statistic_units_created >= amount

        return wrap

    @staticmethod
    def _num_footman(amount, player=0):
        def wrap(self):
            p = self.game.players[player]
            return p.num_footman >= amount

        return wrap

    @staticmethod
    def _num_peasant(amount, player=0):
        def wrap(self):
            p = self.game.players[player]
            return p.num_peasant >= amount

        return wrap

    @staticmethod
    def _num_archer(amount, player=0):
        def wrap(self):
            p = self.game.players[player]
            return p.num_archer >= amount

        return wrap

    @staticmethod
    def _num_farm(amount, player=0):
        def wrap(self):
            p = self.game.players[player]
            return p.num_farm >= amount

        return wrap

    @staticmethod
    def _num_barracks(amount, player=0):
        def wrap(self):
            p = self.game.players[player]
            return p.num_barrack >= amount

        return wrap

    @staticmethod
    def _num_town_hall(amount, player=0):
        def wrap(self):
            p = self.game.players[player]
            return p.num_town_hall >= amount

        return wrap

    def reset(self):
        self.game.reset()
        self.game.update()
        self.game.render()
        return self.game.get_state()

    def step(self, action):
        player = self.game.selected_player  # py::return_value_policy::reference
        player.do_action(action)

        self.game.update()
        self.game.render()

        s1 = self.game.get_state()
        t, tarr = self.evaluate()
        r = 1 if t else 0

        return s1, r, t, {}

    def render(self, mode='human'):
        if mode == "human":
            self.game.view()

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
