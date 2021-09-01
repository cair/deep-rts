from functools import partial
import numpy as np
import typing
from tabulate import tabulate
from DeepRTS import Engine, Constants
from DeepRTS.python import util, Config, Game

from python import DeepRTSPlayer


class ScenarioData:

    def __init__(self):
        self.previous_statistic_gathered_gold = 0
        self.previous_statistic_gathered_lumber = 0
        self.previous_statistic_food = 0
        self.previous_statistic_damage_done = 0
        self.previous_statistic_damage_taken = 0
        self.previous_num_footman = 0
        self.previous_num_archer = 0
        self.previous_num_peasant = 0
        self.previous_num_town_hall = 0
        self.previous_num_farm = 0
        self.previous_num_barrack = 0
        self.stepCountMax = 1000
        self.stepCounter = 0


    def reset(self):
        self.__init__()


class Scenario(Game):
    DEFAULTS = dict(
        updates_per_action=1,
        state_flatten=False,
        stats_print_interval=10000,
        stats_print=True,
        stats_print_on_terminal=True,
        default_reward_function=False
    )

    def __init__(self, config, *scenarios, config_override=None):
        config_override = config_override if isinstance(config_override, dict) else {}
        util.apply_overrides(config, config_override)

        self.scenario_config = util.dict_update(Scenario.DEFAULTS.copy(), config)
        self.stats_print_interval = self.scenario_config.get("stats_print_interval")
        self.stats_print = self.scenario_config.get("stats_print")
        self.stats_print_on_terminal = self.scenario_config.get("stats_print_on_terminal")

        engine_conf, gui_conf, scenario_conf, engine_config, gui_config = self.read_config(self.scenario_config)

        c_n_players = engine_conf["n_players"] if "n_players" in engine_conf else 1
        c_fps = engine_conf["fps"] if "fps" in engine_conf else -1
        c_ups = engine_conf["ups"] if "ups" in engine_conf else -1
        c_map = engine_conf["map"]
        c_terminal_signal = self.scenario_config["default_reward_function"]

        super().__init__(
            c_map,
            n_players=c_n_players,
            engine_config=engine_config,
            gui_config=gui_config,
            terminal_signal=c_terminal_signal
        )

        self.set_max_fps(c_fps)
        self.set_max_ups(c_ups)

<<<<<<< refs/remotes/origin/wip_2021
        # Struct that holds all data that needs to be stored during a episode
        self.data = ScenarioData()

        # Binds scenario requirements to *this*
        self.scenarios_eval_fns = [partial(scenario["eval"], self) for scenario in scenarios]
        self.scenarios_stat_fns = [partial(scenario["stats"], self) for scenario in scenarios]

        self.terminal = False
        self.winner = None
        self.last_reward = 0
        self._step_count = 0

    def read_config(self, config):
        engine_conf = config["engine"] if "engine" in config else {}
        gui_conf = config["gui"] if "gui" in config else {}
        scenario_conf = config["scenario"] if "scenario" in config else {}

        gui_config = Config(
            render=util.config(gui_conf, "render", True),
            view=util.config(gui_conf, "view", True),
            inputs=util.config(gui_conf, "inputs", False),
            caption=util.config(gui_conf, "caption", False),
            unit_health=util.config(gui_conf, "unit_health", False),
            unit_outline=util.config(gui_conf, "unit_outline", False),
            unit_animation=util.config(gui_conf, "unit_animation", False),
            audio=util.config(gui_conf, "audio", False),
            audio_volume=util.config(gui_conf, "audio_volume", 50)
        )
        engine_config: Engine.Config = Engine.Config.defaults()
        engine_config.set_barracks(util.config(engine_conf, "unit_barracks", True))
        engine_config.set_footman(util.config(engine_conf, "unit_footman", True))
        engine_config.set_instant_town_hall(util.config(engine_conf, "unit_town_hall", True))
        engine_config.set_archer(util.config(engine_conf, "unit_archer", False))
        engine_config.set_start_lumber(util.config(engine_conf, "start_lumber", 0))  # Enough to create TownHall
        engine_config.set_start_gold(util.config(engine_conf, "start_gold", 0))  # Enough to create TownHall
        engine_config.set_start_stone(util.config(engine_conf, "start_stone", 0))
        engine_config.set_tick_modifier(util.config(engine_conf, "tick_modifier", engine_config.tick_modifier))

        return engine_conf, gui_conf, scenario_conf, engine_config, gui_config

    def is_terminal(self):
        return self.terminal

    def print_stats(self):
        full_output = []
        for player in self.players:
            output = [[self._step_count] + scenario(player=player) for scenario in self.scenarios_stat_fns]
            full_output.extend(output)
        print(tabulate(full_output, headers=["Step", "Desc", "Player ID", "Current", "Target", "Goal"],
                       showindex="always", tablefmt="simple"))

    def evaluate(self, player: DeepRTSPlayer):
        """
        Evaluates scenario criteria for a specific player. This does NOT process the game further
        :param player: DeepRTSPlayer
        :return: terminal_signal, last_received reward
        """
        if self.terminal:
            return True, self.last_reward

        successes, rewards = zip(*[scenario(player=player) for scenario in self.scenarios_eval_fns])
        self.terminal = all(successes)
        self.last_reward = sum(rewards)

        if (self.stats_print_on_terminal and self.terminal) or (
                self.stats_print and self._step_count % self.stats_print_interval == 0):
            self.print_stats()

        self._step_count += 1

        if self.terminal and self.winner is None:
            self.winner = player

        return self.terminal, self.last_reward
=======
    def evaluate(self):
        success, reward = zip(*[scenario() for scenario in self.scenarios])
        return any(success), sum(reward)
>>>>>>> Scenario Additions/Cleanup

    def _optimal_play_sequence(self):
        raise NotImplementedError("The function '_optimal_play_sequence' must be implemented!")

    def _optimal_play_gamestep(self, player, total_steps=0, total_reward=0):
        """
        Calculate optimal play for the current player. This PROCESSES the game.
        :param player:
        :param total_steps:
        :param total_reward:
        :return:
        """
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
                total_steps, total_reward, terminal = self._optimal_play_gamestep(player, total_steps, total_reward)

            player.do_action(action)

        terminal = False
        while not terminal:
            total_steps, total_reward, terminal = self._optimal_play_gamestep(player, total_steps, total_reward)

        self.reset()

        return total_steps, total_reward

<<<<<<< refs/remotes/origin/wip_2021
    def reset(self):
        self.data.reset()
        super().reset()
        self.update()
        self.render()
        self.winner = None
        self.terminal = False
        self._step_count = 0
        return self.get_state()
=======
    @staticmethod
    def _gold_collect(amount, player=0):
        def wrap(self):
            p = self.game.players[player]
            return p.statistic_gathered_gold >= amount

        return wrap

    def gold_increment(amount=100, reward_success=1, reward_fail=-0.003, player=0):
        def wrap(self):
            p = self.game.selected_player
            diff = p.statistic_gathered_gold - self.data.previous_statistic_gathered_gold
            self.data.previous_statistic_gathered_gold = p.statistic_gathered_gold
            r = reward_success if diff > 0 else reward_fail
            t = False
            return t, r

        return wrap

    def farm_increment(amount=5, reward_success=100, reward_fail=-0.003, player=0):
        def wrap(self):
            p = self.game.selected_player
            diff = p.num_farm - self.data.previous_num_farm
            self.data.previous_num_farm = p.num_farm
            r = reward_success if diff > 0 else reward_fail
            t = False

            return t, r

        return wrap

    def lumber_increment(amount=100, reward_success=1, reward_fail=-0.003, player=0):
        def wrap(self):
            p = self.game.selected_player
            diff = p.statistic_gathered_lumber - self.data.previous_statistic_gathered_lumber
            self.data.previous_statistic_gathered_lumber = p.statistic_gathered_lumber
            r = reward_success if diff > 0 else reward_fail
            t = False
            return t, r

        return wrap

    def damage_done_increment(amount=100, reward_success=1, reward_fail=-0.003, player=0):
        def wrap(self):
            p = self.game.selected_player
            diff = p.statistic_damage_done - self.data.previous_statistic_damage_done
            self.data.previous_statistic_damage_done = p.statistic_damage_done
            r = reward_success if diff > 0 else reward_fail
            t = False
            return t, r

        return wrap

    def damage_taken_increment(amount=100, reward_success=0.003, reward_fail=-1, player=0):
        def wrap(self):
            p = self.game.selected_player
            diff = p.statistic_damage_taken - self.data.previous_statistic_damage_taken
            self.data.previous_statistic_damage_taken = p.statistic_damage_taken
            r = reward_success if diff > 0 else reward_fail
            t = False
            return t, r

        return wrap

    def peasant_increment(amount=5, reward_success=50, reward_fail=-0.003, player=0):
        def wrap(self):
            p = self.game.selected_player
            diff = p.num_peasant - self.data.previous_num_peasant
            self.data.previous_num_peasant = p.num_peasant
            r = reward_success if diff > 0 else reward_fail
            t = False
            return t, r

        return wrap

    def footman_increment(amount=5, reward_success=50, reward_fail=-0.003, player=0):
        def wrap(self):
            p = self.game.selected_player
            diff = p.num_footman - self.data.previous_num_footman
            self.data.previous_num_footman = p.num_footman
            r = reward_success if diff > 0 else reward_fail
            t = False
            return t, r

        return wrap

    def archer_increment(amount=5, reward_success=50, reward_fail=-0.003, player=0):
        def wrap(self):
            p = self.game.selected_player
            diff = p.num_archer - self.data.previous_num_archer
            self.data.previous_num_archer = p.num_archer
            r = reward_success if diff > 0 else reward_fail
            t = False
            return t, r

        return wrap

    def game_end(amount=100, reward_success=10000, reward_fail=-1000, player=0):
        def wrap(self):
            t = False
            r = 0
            if(self.game.selected_player == self.game.players[0]):
                p = self.game.players[0]
                if (p.is_defeated()):
                    t = True
                    r = reward_fail
                elif (self.game.players[1].is_defeated()):
                    t = True
                    r = reward_success
                return t, r

            if (self.game.selected_player == self.game.players[1]):
                p = self.game.players[1]
                if (p.is_defeated()):
                    t = True
                    r = reward_fail
                elif (self.game.players[0].is_defeated()):
                    t = True
                    r = reward_success
                return t, r

        return wrap

    def food_increment(amount=50, reward_success=1, reward_fail=-0.003, player=0):
        def wrap(self):
            p = self.game.selected_player
            diff = p.food - self.data.previous_statistic_food
            self.data.previous_statistic_food = p.food
            r = reward_success if diff > 0 else reward_fail
            t = False
            return t, r

        return wrap

    def town_hall_increment(amount=3, reward_success=100, reward_fail=-0.003, player=0):
        def wrap(self):
            p = self.game.selected_player
            diff = p.num_town_hall - self.data.previous_num_town_hall
            self.data.previous_num_town_hall = p.num_town_hall
            r = reward_success if diff > 0 else reward_fail
            t = False

            return t, r

        return wrap

    def barrack_increment(amount=3, reward_success=100, reward_fail=-0.003, player=0):
        def wrap(self):
            p = self.game.selected_player
            diff = p.num_barrack - self.data.previous_num_barrack
            self.data.previous_num_barrack = p.num_barrack
            r = reward_success if diff > 0 else reward_fail
            t = False

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
>>>>>>> Scenario Additions/Cleanup

    def step(self, action: typing.Union[dict, int]) -> (
            typing.Union[typing.Dict[int, np.ndarray], np.ndarray],  # State
            typing.Union[typing.Dict[int, float], float],   # Reward
            typing.Union[typing.Dict[int, bool], bool],  # Terminal
            typing.Union[typing.Dict[int, dict], dict]  # Info
    ):

        if isinstance(action, int):
            player = self.selected_player
            player.do_action(action)
            for _ in range(self.scenario_config["updates_per_action"]):
                self.update()

            s1 = player.get_state()
            t, r = self.evaluate(player=player)
            return s1, r, t, {}

        elif isinstance(action, dict):
            # Make action
            for player_idx, player_action in action.items():
                player = self.players[player_idx]
                player.do_action(player_action)

            # Process game
            for _ in range(self.scenario_config["updates_per_action"]):
                self.update()

            # Get state for players.
            states = {}
            rewards = {}
            terminals = {}
            infos = {}
            for player_idx, player_action in action.items():
                player = self.players[player_idx]
                s1 = player.get_state()
                t, r = self.evaluate(player=player)
                states[player_idx] = s1
                rewards[player_idx] = r
                terminals[player_idx] = t
                infos[player_idx] = {}
            return states, rewards, terminals, infos

        else:
            raise TypeError(f"Action type is incorrect. excpected list or str. Got {type(action)}")








        return s1, r, t, {}

    def render(self, mode='human'):
        if mode == "human":
<<<<<<< refs/remotes/origin/wip_2021
            self.view()


=======
            self.game.view()

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
>>>>>>> Scenario Additions/Cleanup
