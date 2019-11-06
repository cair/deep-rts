from DeepRTS.python.scenario.engine import Scenario
from DeepRTS import python
from DeepRTS import Engine


class GoldCollectOnePlayerFifteen(Scenario):

    def __init__(self, config):

        gui_config = python.Config(
            render=True,
            view=True,
            inputs=True,
            caption=True,
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
        engine_config.set_start_wood(250) # Enough to create TownHall
        engine_config.set_start_gold(500)  # Enough to create TownHall
        engine_config.set_start_oil(0)

        game = python.Game(
            python.Config.Map.FIFTEEN,
            n_players=1,
            engine_config=engine_config,
            gui_config=gui_config,
            terminal_signal=False
        )

        super().__init__(
            game,
            Scenario.GOLD_COLLECT(1000)
        )