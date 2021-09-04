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

    def reset(self):
        self.data.reset()
        super().reset()
        self.update()
        self.render()
        self.winner = None
        self.terminal = False
        self._step_count = 0
        return self.get_state()

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
            self.view()


