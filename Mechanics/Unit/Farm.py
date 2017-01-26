from Mechanics.Constants import Unit as UnitC
from Mechanics.Unit.Unit import Unit


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


    def __init__(self, player):
        super().__init__(player)
        self.player = player

        self.can_build = []



