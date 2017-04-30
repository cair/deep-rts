from game import const
from game import Config
from game.logic.Unit.Unit import Unit

class Peasant(Unit):

    # Identifier
    id = const.Unit.ID_PEASANT
    structure = const.Unit.TYPE_UNIT

    # Tile Data
    width = 1
    height = 1

    # Economics
    cost_lumber = 0
    cost_gold = 400
    build_duration = 45 * Config.FRAME_MULTIPLIER

    # Stats
    name = "Peasant"
    damage_min = 2
    damage_max = 9
    damage_piercing = 2
    damage_range = 1
    armor = 0
    health_max = 30
    speed = 10
    sight = 4

    can_harvest = True

    def __init__(self, player, init=True):
        super().__init__(player, init)
        self.player = player

        self.buildable = [
            self.game.UnitManager.UNITS[self.game.UnitManager.BASE_1][player.race],
            self.game.UnitManager.UNITS[self.game.UnitManager.FARM][player.race],
            self.game.UnitManager.UNITS[self.game.UnitManager.BARRACKS][player.race]
        ]



