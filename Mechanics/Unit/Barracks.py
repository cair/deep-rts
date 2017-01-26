from Mechanics.Constants import Unit as UnitC
from Mechanics.Unit.Unit import Unit


class Barracks(Unit):

    # Identifier
    id = UnitC.BARRACKS
    structure = UnitC.STRUCTURE

    # Tile Data
    width = 3
    height = 3

    # Economics
    cost_lumber = 450
    cost_gold = 700

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


    def __init__(self, player):
        super().__init__(player)
        self.player = player

        self.can_build = [
            player.unit_manager.UNITS[player.unit_manager.SOLDIER][player.race]
        ]



