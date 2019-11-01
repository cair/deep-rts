from DeepRTS.python.scenario.engine import Scenario
from DeepRTS import python


class GoldThousand(Scenario):

    def __init__(self, game: python.Game, *scenarios):
        super().__init__(game,
                         Scenario.GOLD_COLLECT(1000)
                         )
