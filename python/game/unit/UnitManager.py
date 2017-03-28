from game import Constants
from game import Config
from game.unit.Unit import Unit

class UnitManager:

    @staticmethod
    def construct_unit(unit_type, player):

        if unit_type == Constants.Unit.TownHall:
            return UnitManager.construct_town_hall(player)
        elif unit_type == Constants.Unit.Peasant:
            return UnitManager.construct_peasant(player)
        else:
            assert False  # Should NEVER HAPPEN

    @staticmethod
    def construct_peasant(player):
        u = Unit(player)

        u.type_id = Constants.Unit.Peasant
        u.health = 30
        u.health_max = 30
        u.direction = 0

        u.ground_unit = True
        u.water_unit = False

        u.damage_min = 2
        u.damage_max = 9
        u.damage_range = 1
        u.damage_piercing = 2
        u.armor = 0

        u.gold_carry = 0
        u.lumber_carry = 0
        u.oil_carry = 0
        u.carry_capacity = 10
        u.speed = 10
        u.sight = 4
        u.range = 1

        u.can_harvest = True
        u.can_attack = True
        u.can_move = True
        u.military = False
        u.structure = False

        u.lumber_cost = 0
        u.gold_cost = 400
        u.oil_cost = 0

        u.spawn_duration = 45 * Config.TICK_MULTIPLIER

        u.food_production = 0
        u.food_consumption = 1

        u.width = 1
        u.height = 1

        u.name = "Peasant"

        u.build_inventory = [Constants.Unit.Peasant]

        return u

    @staticmethod
    def construct_town_hall(player):
        u = Unit(player)
        u.type_id = Constants.Unit.TownHall
        u.health = 1200
        u.health_max = 1200
        u.direction = 0

        u.ground_unit = True
        u.water_unit = False

        u.damage_min = 0
        u.damage_max = 0
        u.damage_range = 0
        u.damage_piercing = 0
        u.armor = 0

        u.gold_carry = 0
        u.lumber_carry = 0
        u.oil_carry = 0
        u.carry_capacity = 0
        u.speed = 0
        u.sight = 4
        u.range = 0

        u.can_harvest = False
        u.can_attack = False
        u.can_move = False
        u.military = False
        u.structure = True
        u.recallable = True

        u.lumber_cost = 250
        u.gold_cost = 500
        u.oil_cost = 0

        u.spawn_duration = 60 * Config.TICK_MULTIPLIER

        u.food_production = 1
        u.food_consumption = 0

        u.width = 3
        u.height = 3

        u.name = "Town-Hall"

        u.build_inventory = [Constants.Unit.Peasant]

        return u




