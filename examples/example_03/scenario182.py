from DeepRTS.python.scenario.engine import Scenario, ScenarioData
from DeepRTS.python import util, Config, Game
from DeepRTS import Engine
import gym
import numpy as np



class Scenario182Data(ScenarioData):

    def __init__(self):
        self.previous_statistic_damage_done = 0
        self.previous_statistic_resources = 0
        self.previous_statistic_buildings = 0
        self.previous_statistic_units = 0
        self.previous_statistic_food = 0
        super().__init__()

    def reset(self):
        self.__init__()

class Scenario182(Scenario):


    def __init__(self, config):

        engconf = config["engine"] if "engine" in config else {}
        gconf = config["gui"] if "gui" in config else {}
        rlconf = config["rl"] if "rl" in config else {}

        gui_config = Config(
            render=util.config(gconf, "render", True),
            view=util.config(gconf, "view", True),
            inputs=util.config(gconf, "inputs", False),
            caption=util.config(gconf, "caption", False),
            unit_health=util.config(gconf, "unit_health", False),
            unit_outline=util.config(gconf, "unit_outline", False),
            unit_animation=util.config(gconf, "unit_animation", False),
            audio=util.config(gconf, "audio", False),
            audio_volume=util.config(gconf, "audio_volume", 50)
        )

        engine_config: Engine.Config = Engine.Config.defaults()
        engine_config.set_instant_building(True)
        engine_config.set_instant_town_hall(True)
        engine_config.set_barracks(True)
        engine_config.set_farm(True)
        engine_config.set_footman(True)
        engine_config.set_archer(True)
        engine_config.set_start_lumber(500)
        engine_config.set_start_gold(500)
        engine_config.set_start_stone(500)
        engine_config.set_tick_modifier(util.config(engconf, "tick_modifier", engine_config.tick_modifier))
        engine_config.set_console_caption_enabled(False)

        game = Game(
            Config.Map.TWENTYONE,
            n_players=2,
            engine_config=engine_config,
            gui_config=gui_config,
            terminal_signal=True
        )

        c_fps = engconf["fps"] if "fps" in engconf else -1
        c_ups = engconf["ups"] if "ups" in engconf else -1

        game.set_max_fps(c_fps)
        game.set_max_ups(c_ups)

        super().__init__(
            rlconf,
            game,

            # Scenarios
            self.GAME_END(),
            self.FOOD(),
            self.RESOURCE_GATHERED(),
            self.BUILDINGS_BUILT(),
            self.UNITS_CREATED(),
            self.DAMAGE_DONE()
            # Scenario.DAMAGE_TAKEN(),
            # Scenario.NUM_FARM(),
            # Scenario.NUM_TOWN_HALL(),
            # Scenario.NUM_BARRACK(),
            # Scenario.GOLD_COLLECT_INCREMENT(100)
            # Scenario.LUMBER_COLLECT_INCREMENT(),
            # Scenario.OIL_COLLECT_INCREMENT(),
            # Scenario.UNITS_CREATED()
        )

        self.data = Scenario182Data()
        self.DEFAULTS['flatten'] = False