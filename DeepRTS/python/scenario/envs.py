from DeepRTS.python.scenario.engine import Scenario
from DeepRTS.python import util, Config, Game
from DeepRTS import Engine


class GoldCollectOnePlayerFifteen(Scenario):

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
        engine_config.set_barracks(True)
        engine_config.set_footman(True)
        engine_config.set_instant_town_hall(True)
        engine_config.set_archer(True)
        engine_config.set_start_lumber(250)  # Enough to create TownHall
        engine_config.set_start_gold(500)  # Enough to create TownHall
        engine_config.set_start_oil(0)
        engine_config.set_tick_modifier(util.config(engconf, "tick_modifier", engine_config.tick_modifier))

        game = Game(
            Config.Map.FIFTEEN,
            n_players=1,
            engine_config=engine_config,
            gui_config=gui_config,
            terminal_signal=False
        )

        c_fps = engconf["fps"] if "fps" in engconf else -1
        c_ups = engconf["ups"] if "ups" in engconf else -1

        game.set_max_fps(c_fps)
        game.set_max_ups(c_ups)

        super().__init__(
            rlconf,
            game,
            #Scenario.GOLD_COLLECT(1000),
            Scenario.GOLD_COLLECT_INCREMENT(100)
        )

    def _optimal_play_sequence(self):
        return [
            (Engine.Constants.Action.MoveRight, "Peasant0"),
            (Engine.Constants.Action.MoveRight, "Peasant0"),
            (Engine.Constants.Action.MoveRight, "Peasant0"),
            (Engine.Constants.Action.MoveRight, "Peasant0"),
            (Engine.Constants.Action.MoveDownRight, "Peasant0"),
            (Engine.Constants.Action.MoveRight, "Peasant0")
        ]
