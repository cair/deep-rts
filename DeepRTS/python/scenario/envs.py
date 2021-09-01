from DeepRTS.python.scenario.engine import Scenario
from DeepRTS.python import util, Config, Game
from DeepRTS import Engine


class GeneralAI_1v1(Scenario,):

    def __init__(self,theMap):
        rlconf = {}

        gui_config = Config(
            render=True,
            view= True,
            inputs=True,
            caption=False,
            unit_health=False,
            unit_outline=False,
            unit_animation=False,
            audio=False,
            audio_volume=50
        )

        engine_config: Engine.Config = Engine.Config.defaults()
        engine_config.set_barracks(True)
        engine_config.set_footman(True)
        engine_config.set_instant_town_hall(True)
        engine_config.set_archer(True)
        engine_config.set_start_lumber(1000)  # Enough to create TownHall
        engine_config.set_start_gold(1000)  # Enough to create TownHall
        engine_config.set_start_oil(10000)
        engine_config.set_tick_modifier(engine_config.tick_modifier)
        engine_config.set_instant_building(False)

        game = Game(
            theMap,
            n_players=2,
            engine_config=engine_config,
            gui_config=gui_config,
            terminal_signal=False
        )

        game.set_max_fps(500000000)
        game.set_max_ups(500000000)


        super().__init__(
            rlconf,
            game,
            Scenario.gold_increment(10000),
            Scenario.lumber_increment(10000),
            Scenario.food_increment(10000),
            Scenario.town_hall_increment(10000),
            Scenario.barrack_increment(10000),
            Scenario.farm_increment(10000),
            Scenario.peasant_increment(10000),
            Scenario.archer_increment(10000),
            Scenario.footman_increment(10000),
            Scenario.damage_done_increment(10000),
            Scenario.game_end()
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
