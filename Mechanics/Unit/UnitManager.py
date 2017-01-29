from Mechanics.Constants import Race
from Mechanics.Unit.Barracks import Barracks
from Mechanics.Unit.Farm import Farm
from Mechanics.Unit.Footman import Footman
from Mechanics.Unit.Unit import Unit
from Mechanics.Unit.Peasant import Peasant
from Mechanics.Unit.TownHall import TownHall


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




