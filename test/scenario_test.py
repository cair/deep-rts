
from functools import partial
from DeepRTS import python
from DeepRTS.Engine import Player


class Scenario:

    def __init__(self, game: python.Game, *scenarios):
        self.game = game

        self.scenarios = [partial(scenario, self) for scenario in scenarios]

    def evaluate(self):
        values = [scenario() for scenario in self.scenarios]
        return all(values), values

    @staticmethod
    def _gold_collect(amount: int, player=0):
        def wrap(self: Scenario):

            p: Player = self.game.players[player]
            return p.statistic_gathered_gold >= amount

        return wrap

    @staticmethod
    def _lumber_collect(amount: int, player=0):
        def wrap(self: Scenario):

            p: Player = self.game.players[player]
            return p.statistic_gathered_lumber >= amount

        return wrap

    @staticmethod
    def _oil_collect(amount: int, player=0):
        def wrap(self: Scenario):

            p: Player = self.game.players[player]
            return p.statistic_gathered_oil >= amount

        return wrap

    GOLD_COLLECT = _gold_collect
    OIL_COLLECT = _lumber_collect
    LUMBER_COLLECT = _oil_collect


if __name__ == "__main__":

    x = Scenario(
        python.Game("15x15-2v2.json", n_players=2),
        Scenario.GOLD_COLLECT(500),
        Scenario.LUMBER_COLLECT(500),
        Scenario.OIL_COLLECT(0)
    )

    print(x.evaluate())