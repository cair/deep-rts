from Mechanics.Constants import Unit as UnitC
from Mechanics.Unit.Unit import Unit


class TownHall(Unit):

    # Identifier
    id = UnitC.TOWN_HALL
    structure = UnitC.STRUCTURE

    # Tile Data
    width = 3
    height = 3

    # Economics
    cost_lumber = 800
    cost_gold = 1200

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


    def __init__(self, player):
        super().__init__(player)
        self.player = player

        self.can_build = [
            player.unit_manager.UNITS[player.unit_manager.WORKER][player.race]
        ]



