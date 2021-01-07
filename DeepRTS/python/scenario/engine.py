from functools import partial
import gym
import numpy as np
from DeepRTS.Engine import Constants
from DeepRTS.python import util
from coding.util import LimitedDiscrete
from tabulate import tabulate


class ScenarioData:

    def __init__(self):
        self.previous_statistic_gathered_gold = 0

    def reset(self):
        self.__init__()


class Scenario(gym.Env):
    DEFAULTS = dict(
        updates_per_action=1,
        flatten=False,
        stats_print_interval=10000,
        stats_print=True,
        stats_print_on_terminal=True
    )

    def __init__(self, config, game, *scenarios):
        self.config = util.dict_update(Scenario.DEFAULTS.copy(), config)
        self.game = game

        # Struct that holds all data that needs to be stored during a episode
        self.data = ScenarioData()

        # Binds scenario requirements to *this*
        self.scenarios_eval_fns = [partial(scenario["eval"], self) for scenario in scenarios]
        self.scenarios_stat_fns = [partial(scenario["stats"], self) for scenario in scenarios]

        # Define the action space
        self.action_space = LimitedDiscrete(Constants.action_min, Constants.action_max)

        # Define the observation space, here we assume that max is 255 (image) # TODO
        self.observation_space = gym.spaces.Box(0, 255, shape=self.get_state().shape, dtype=np.float32)

        self.terminal = False
        self.winner = None
        self.last_reward = 0
        self._step_count = 0
        self.stats_print_interval = self.config.get("stats_print_interval")
        self.stats_print = self.config.get("stats_print")
        self.stats_print_on_terminal = self.config.get("stats_print_on_terminal")

    def is_terminal(self):
        return self.terminal

    def print_stats(self):
        full_output = []
        for player in self.game.players:
            output = [[self._step_count] + scenario(player=player) for scenario in self.scenarios_stat_fns]
            full_output.extend(output)
        print(tabulate(full_output, headers=["Step", "Desc", "Player ID", "Current", "Target", "Goal"],
                       showindex="always", tablefmt="simple"))

    def evaluate(self, player):
        if self.terminal:
            return True, self.last_reward

        successes, rewards = zip(*[scenario(player=player) for scenario in self.scenarios_eval_fns])
        self.terminal = all(successes)
        self.last_reward = sum(rewards)

        if (self.stats_print_on_terminal and self.terminal) or (self.stats_print and self._step_count % self.stats_print_interval == 0):
            self.print_stats()

        self._step_count += 1

        if self.terminal and self.winner is None:
            self.winner = player

        return self.terminal, self.last_reward

    def _optimal_play_sequence(self):
        raise NotImplementedError("The function '_optimal_play_sequence' must be implemented!")

    def _optimal_play_gamestep(self, player, total_steps=0, total_reward=0):
        for _ in range(self.config["updates_per_action"]):
            self.game.update()
        t, r = self.evaluate(player=player)  # TODO
        total_reward += r
        total_steps += 1
        return total_steps, total_reward, t

    def calculate_optimal_play(self):
        self.reset()
        player = self.game.selected_player
        total_steps, total_reward, terminal = self._optimal_play_gamestep(player=player)

        initial_build_handled = False
        for action, unitID in self._optimal_play_sequence():
            unit = self.game.get_unit_by_name_id(unitID)

            if not initial_build_handled:
                while unit.state.id == Constants.State.Building:
                    self._optimal_play_gamestep(player=player)
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
    def _gold_collect(target):
        def eval_wrapper(self, player):
            t = player.statistic_gathered_gold >= target
            r = 1 if t else 0
            return t, r

        def stats_wrapper(self, player):
            t, r = eval_wrapper(self, player)
            return ["Gold Collected", player.get_id(), player.statistic_gathered_gold, target, t]

        return dict(
            eval=eval_wrapper,
            stats=stats_wrapper
        )

    @staticmethod
    def _gold_collect_increment(amount, reward_success=1, reward_fail=-0.01):
        def eval_wrapper(self, player):
            diff = player.statistic_gathered_gold - self.data.previous_statistic_gathered_gold
            self.data.previous_statistic_gathered_gold = player.statistic_gathered_gold
            r = reward_success if diff > 0 else reward_fail
            t = player.statistic_gathered_gold >= amount
            return t, r

        def stats_wrapper(self, player):
            return []

        return dict(
            eval=eval_wrapper,
            stats=stats_wrapper
        )

    @staticmethod
    def _lumber_collect(amount):
        def eval_wrapper(self, player):
            p = self.game.players[player]
            return p.statistic_gathered_lumber >= amount

        def stats_wrapper(self, player):
            return []

        return dict(eval=eval_wrapper,
                    stats=stats_wrapper
                    )

    @staticmethod
    def _oil_collect(amount, player=0):
        def eval_wrapper(self, player):
            p = self.game.players[player]
            return p.statistic_gathered_oil >= amount

        def stats_wrapper(self, player):
            return []

        return dict(eval=eval_wrapper,
                    stats=stats_wrapper
                    )

    @staticmethod
    def _food_consumption(amount, player=0):
        def eval_wrapper(self, player):
            p = self.game.players[player]
            return p.food_consumption >= amount

        def stats_wrapper(self, player):
            return []

        return dict(
            eval=eval_wrapper,
            stats=stats_wrapper
        )

    @staticmethod
    def _food_count(amount, player=0):
        def eval_wrapper(self, player):
            p = self.game.players[player]
            return p.food >= amount

        def stats_wrapper(self, player):
            return []

        return dict(
            eval=eval_wrapper,
            stats=stats_wrapper
        )

    @staticmethod
    def _damage_done(amount, player=0):
        def eval_wrapper(self, player):
            p = self.game.players[player]
            return p.statistic_damage_done >= amount

        def stats_wrapper(self, player):
            return []

        return dict(
            eval=eval_wrapper,
            stats=stats_wrapper
        )

    @staticmethod
    def _damage_taken(amount, player=0):
        def eval_wrapper(self, player):
            p = self.game.players[player]
            return p.statistic_damage_taken >= amount

        def stats_wrapper(self, player):
            return []

        return dict(
            eval=eval_wrapper,
            stats=stats_wrapper
        )

    @staticmethod
    def _units_created(amount, player=0):
        def eval_wrapper(self, player):
            p = self.game.players[player]
            return p.statistic_units_created >= amount

        def stats_wrapper(self, player):
            return []

        return dict(
            eval=eval_wrapper,
            stats=stats_wrapper
        )

    @staticmethod
    def _num_footman(amount, player=0):
        def eval_wrapper(self, player):
            p = self.game.players[player]
            return p.num_footman >= amount

        def stats_wrapper(self, player):
            return []

        return dict(
            eval=eval_wrapper,
            stats=stats_wrapper
        )

    @staticmethod
    def _num_peasant(amount, player=0):
        def eval_wrapper(self, player):
            p = self.game.players[player]
            return p.num_peasant >= amount

        def stats_wrapper(self, player):
            return []

        return dict(
            eval=eval_wrapper,
            stats=stats_wrapper
        )

    @staticmethod
    def _num_archer(amount, player=0):
        def eval_wrapper(self, player):
            p = self.game.players[player]
            return p.num_archer >= amount

        def stats_wrapper(self, player):
            return []

        return dict(
            eval=eval_wrapper,
            stats=stats_wrapper
        )

    @staticmethod
    def _num_farm(amount, player=0):
        def eval_wrapper(self, player):
            p = self.game.players[player]
            return p.num_farm >= amount

        def stats_wrapper(self, player):
            return []

        return dict(
            eval=eval_wrapper,
            stats=stats_wrapper
        )

    @staticmethod
    def _num_barracks(amount, player=0):
        def eval_wrapper(self, player):
            p = self.game.players[player]
            return p.num_barrack >= amount

        def stats_wrapper(self, player):
            return []

        return dict(
            eval=eval_wrapper,
            stats=stats_wrapper
        )

    @staticmethod
    def _num_town_hall(amount, player=0):
        def eval_wrapper(self, player):
            p = self.game.players[player]
            return p.num_town_hall >= amount

        def stats_wrapper(self, player):
            return []

        return dict(
            eval=eval_wrapper,
            stats=stats_wrapper
        )

    def reset(self):
        self.data.reset()
        self.game.reset()
        self.game.update()
        self.game.render()
        self.winner = None
        self.terminal = False
        self._step_count = 0
        return self.get_state()

    @property
    def players(self):
        return self.game.players

    def get_state(self, image=False, copy=False):
        if self.config["flatten"]:
            return self.game.get_state(image=image, copy=copy).flatten()
        return self.game.get_state(image=image, copy=copy)

    def step(self, action, player=None):
        selected_player = player if player else self.game.selected_player  # py::return_value_policy::reference
        selected_player.do_action(action + 1)

        for _ in range(self.config["updates_per_action"]):
            self.game.update()

        s1 = self.get_state()
        t, r = self.evaluate(player=selected_player)
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
