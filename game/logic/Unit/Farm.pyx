from game.const import Unit as UnitC
from game import Config
from game.logic.Unit.Unit import Unit


class Farm(Unit):

    # Identifier
    id = UnitC.FARM
    structure = UnitC.STRUCTURE

    # Tile Data
    width = 1
    height = 1

    # Economics
    cost_lumber = 250
    cost_gold = 500
    build_duration = 255 * Config.FRAME_MULTIPLIER

    # Stats
    name = "Farm"
    damage_min = 0
    damage_max = 0
    damage_piercing = 0
    damage_range = 0
    armor = 20
    health = 400
    health_max = 400
    speed = 0
    sight = 3
    food = 4
    food_cost = 0

    def __init__(self, player, attrs={}):
        super().__init__(player, attrs)
        self.player = player

        self.buildable = []



