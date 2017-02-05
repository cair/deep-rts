from Mechanics.Constants import Config
from Mechanics.Unit import Unit


class TownHall(Unit.Unit):

    # Identifier
    id = Unit.TOWN_HALL
    structure = Unit.STRUCTURE

    # Tile Data
    width = 3
    height = 3

    # Economics
    cost_lumber = 800
    cost_gold = 1200
    build_duration = 255 * Config.FRAME_MULTIPLIER

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
    food = 5
    food_cost = 0


    def __init__(self, player, attrs={}):
        super().__init__(player, attrs)
        self.player = player

        self.buildable = [
            self.game.UnitManager.UNITS[self.game.UnitManager.WORKER][player.race]
        ]

