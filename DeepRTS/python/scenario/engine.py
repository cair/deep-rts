from functools import partial
import gym
import numpy as np
from DeepRTS.Engine import Constants
from DeepRTS.python import util
from coding.util import LimitedDiscrete


class ScenarioData:

    def __init__(self):
        self.previous_statistic_gathered_gold = 0

    def reset(self):
        self.__init__()


class Scenario(gym.Env):
    DEFAULTS = dict(
        updates_per_action=1,
        flatten=True
    )

    def __init__(self, config, game, *scenarios):
        self.config = util.dict_update(Scenario.DEFAULTS.copy(), config)
        self.game = game

        # Struct that holds all data that needs to be stored during a episode
        self.data = ScenarioData()

        # Binds scenario requirements to *this*
        self.scenarios = [partial(scenario, self) for scenario in scenarios]

        # Define the action space
        self.action_space = LimitedDiscrete(Constants.action_min, Constants.action_max)

        # Define the observation space, here we assume that max is 255 (image) # TODO
        self.observation_space = gym.spaces.Box(0, 255, shape=self.get_state().shape, dtype=np.float32)

    def evaluate(self):
        success, reward = zip(*[scenario() for scenario in self.scenarios])
        return all(success), sum(reward)

    def _optimal_play_sequence(self):
        raise NotImplementedError("The function '_optimal_play_sequence' must be implemented!")

    def _optimal_play_gamestep(self, total_steps=0, total_reward=0):
        for _ in range(self.config["updates_per_action"]):
            self.game.update()
        t, r = self.evaluate()
        total_reward += r
        total_steps += 1
        return total_steps, total_reward, t

    def calculate_optimal_play(self):
        self.reset()
        player = self.game.selected_player
        total_steps, total_reward, terminal = self._optimal_play_gamestep()

        initial_build_handled = False
        for action, unitID in self._optimal_play_sequence():
            unit = self.game.get_unit_by_name_id(unitID)

            if not initial_build_handled:
                while unit.state.id == Constants.State.Building:
                    self._optimal_play_gamestep()
                initial_build_handled = True

            if not unit:
                raise RuntimeError("Error in optimal_play_sequence. The unit with ID=%s was not found." % unitID)

            player.set_targeted_unit_id(unit.id)

            # Process game while unit is not idle
            while unit.state.id != Constants.State.Idle:  # TODO - convert from getter to property in binding
                total_steps, total_reward, terminal = self._optimal_play_gamestep(total_steps, total_reward)

            player.do_action(action)

        terminal = False
        while not terminal:
            total_steps, total_reward, terminal = self._optimal_play_gamestep(total_steps, total_reward)

        self.reset()

        return total_steps, total_reward

    @staticmethod
    def _gold_collect(amount, player=0):
        def wrap(self):
            p = self.game.players[player]
            return p.statistic_gathered_gold >= amount

        return wrap

    @staticmethod
    def _gold_collect_increment(amount, reward_success=1, reward_fail=-0.01, player=0):
        def wrap(self):
            p = self.game.players[player]
            diff = p.statistic_gathered_gold - self.data.previous_statistic_gathered_gold
            self.data.previous_statistic_gathered_gold = p.statistic_gathered_gold
            r = reward_success if diff > 0 else reward_fail
            t = p.statistic_gathered_gold >= amount
            return t, r

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
        self.data.reset()
        self.game.reset()
        self.game.update()
        self.game.render()
        return self.get_state()

    def get_state(self):
        if self.config["flatten"]:
            return self.game.get_state().flatten()
        return self.game.get_state()

    def step(self, action):
        player = self.game.selected_player  # py::return_value_policy::reference
        player.do_action(action + 1)

        for _ in range(self.config["updates_per_action"]):
            self.game.update()

        self.game.render()

        s1 = self.get_state()
        t, r = self.evaluate()
        return s1, r, t, {}

    def render(self, mode='human'):
        if mode == "human":
            self.game.view()

    GOLD_COLLECT = _gold_collect
    GOLD_COLLECT_INCREMENT = _gold_collect_increment
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
