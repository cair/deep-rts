from game import const

from game.unit.Unit import Unit


class TownHall(Unit):

    # Identifier
    id = const.Unit.ID_TOWN_HALL
    structure = const.Unit.TYPE_STRUCTURE

    # Tile Data
    width = 3
    height = 3

    # Economics
    cost_lumber = 800
    cost_gold = 1200
    build_duration = 1#255 * Config.FRAME_MULTIPLIER

    # Stats
    name = "Town Hall"
    damage_min = 0
    damage_max = 0
    damage_piercing = 0
    damage_range = 0
    armor = 0
    health = 1200
    health_max = 1200
    speed = 0
    sight = 4
    food = 1
    food_cost = 0

    def __init__(self, player, init=True):
        super().__init__(player, init)
        self.player = player

        self.buildable = [
            self.game.UnitManager.UNITS[self.game.UnitManager.WORKER][player.race]
        ]

