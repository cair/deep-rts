

from Mechanics.Constants import Unit as UnitC, Config
from Mechanics.Unit.Unit import Unit


class Footman(Unit):

    # Identifier
    id = UnitC.FOOTMAN
    structure = UnitC.UNIT

    # Tile Data
    width = 1
    height = 1

    # Economics
    cost_lumber = 0
    cost_gold = 600
    build_duration = 255 * Config.FRAME_MULTIPLIER

    # Stats
    name = "Footman"
    damage_min = 6
    damage_max = 6
    damage_piercing = 3
    damage_range = 1
    armor = 2
    health = 60
    health_max = 60
    speed = 10
    sight = 4

    def __init__(self, player, attrs={}):
        super().__init__(player, attrs)
        self.player = player

        self.buildable = [
        ]




