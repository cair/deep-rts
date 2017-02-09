

from game.const import Race
from game.const import Unit as UnitC
from game.logic.Unit.Barracks import Barracks
from game.logic.Unit.Farm import Farm
from game.logic.Unit.Footman import Footman
from game.logic.Unit.Peasant import Peasant
from game.logic.Unit.TownHall import TownHall
from game.logic.Unit.Unit import Unit


class UnitManager:
    WORKER = 0
    BASE_1 = 1
    FARM = 2
    SOLDIER = 3
    BARRACKS = 4

    UnitC = UnitC

    UNIT_MAP = {
        UnitC.FOOTMAN: Footman,
        UnitC.PEASANT: Peasant,
        UnitC.BARRACKS: Barracks,
        UnitC.TOWN_HALL: TownHall,
        UnitC.FARM: Farm
    }

    UNITS = {
        WORKER: {
            Race.HUMAN: Peasant,
            Race.ORC: Peasant
        },
        BASE_1: {
            Race.HUMAN: TownHall,
            Race.ORC: TownHall
        },
        FARM: {
            Race.HUMAN: Farm,
            Race.ORC: Farm
        },
        SOLDIER: {
            Race.HUMAN: Footman,
            Race.ORC: Footman
        },
        BARRACKS: {
            Race.HUMAN: Barracks,
            Race.ORC: Barracks
        }
    }

    @staticmethod
    def worker(player):
        return UnitManager.UNITS[UnitManager.WORKER][player.race](player)

    @staticmethod
    def soldier(player):
        return UnitManager.UNITS[UnitManager.SOLDIER][player.race](player)

    @staticmethod
    def get_class_by_id(unit_type_id):
        clazz = UnitManager.UNIT_MAP[unit_type_id]
        return clazz





