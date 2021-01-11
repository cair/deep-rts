from DeepRTS.python.scenario.engine import Scenario
from DeepRTS.python.scenario import functions
from DeepRTS import Engine
from python import Config


class GoldCollectFifteen(Scenario):

    def __init__(self, config):
        super().__init__(
            config,
            functions.GOLD_COLLECT(10),
            config_override=dict(
                engine=dict(
                    map=Config.Map.FIND_GOLD
                )
            )
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
