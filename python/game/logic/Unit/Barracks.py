from game import Config
from game.logic.Unit.Unit import Unit
from game import const

class Barracks(Unit):

    # Identifier
    id = const.Unit.ID_BARRACKS
    structure = const.Unit.TYPE_STRUCTURE
    military = True

    # Tile Data
    width = 3
    height = 3

    # Economics
    cost_lumber = 450
    cost_gold = 700
    build_duration = 200 * Config.FRAME_MULTIPLIER

    # Stats
    name = "Barracks"
    damage_min = 0
    damage_max = 0
    damage_piercing = 0
    damage_range = 0
    armor = 20
    health = 800
    health_max = 800
    speed = 0
    sight = 3
    food = 1

    def __init__(self, player, init=True):
        super().__init__(player, init)
        self.player = player

        self.buildable = [
            self.game.UnitManager.UNITS[self.game.UnitManager.SOLDIER][player.race]
        ]



