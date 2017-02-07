

from game.const import Race
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

    UNITS = {
        WORKER: {
            Race.HUMAN: Peasant,
            Race.ORC: Unit  # TODO
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




