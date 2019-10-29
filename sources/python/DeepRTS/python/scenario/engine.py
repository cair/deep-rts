

class ScenarioEngine:

    @staticmethod
    def gold_collected(player, amount):
        return lambda: player.statistic_gathered_gold == amount

    @staticmethod
    def oil_collected(player, amount):
        return lambda: player.statistic_gathered_oil == amount

    @staticmethod
    def lumber_collected(player, amount):
        return lambda: player.statistic_gathered_lumber == amount

    @staticmethod
    def build(self, ):