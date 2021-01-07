from DeepRTS.python.scenario.engine import Scenario
from DeepRTS.python import util, Config, Game
from DeepRTS import Engine


def read_config(scenario, config):
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
    engine_config.set_start_oil(util.config(engine_conf, "start_oil", 0))
    engine_config.set_tick_modifier(util.config(engine_conf, "tick_modifier", engine_config.tick_modifier))

    c_n_players = engine_conf["n_players"] if "n_players" in engine_conf else 1
    c_fps = engine_conf["fps"] if "fps" in engine_conf else -1
    c_ups = engine_conf["ups"] if "ups" in engine_conf else -1
    if "map" not in engine_conf:
        raise RuntimeError("No map was loaded during configuration. PLease set 'map' in engine config")
    c_map = engine_conf["map"]

    game = Game(
        c_map,
        n_players=c_n_players,
        engine_config=engine_config,
        gui_config=gui_config,
        terminal_signal=False
    )
    game.set_max_fps(c_fps)
    game.set_max_ups(c_ups)

    return game, engine_conf, gui_conf, scenario_conf, engine_config, gui_config


class GoldCollectFifteen(Scenario):

    def __init__(self, config):

        config["engine"]["map"] = Config.Map.FIFTEEN

        game, engine_conf, gui_conf, scenario_conf, engine_config, gui_config = read_config(
            self,
            config
        )

        super().__init__(
            scenario_conf,
            game,
            Scenario.GOLD_COLLECT(10)
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
