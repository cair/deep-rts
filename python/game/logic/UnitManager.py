from game import const
from game.logic import Unit


class UnitManager:
    WORKER = 0
    BASE_1 = 1
    FARM = 2
    SOLDIER = 3
    BARRACKS = 4


    UNIT_MAP = {
        const.Unit.ID_FOOTMAN: Unit.Footman,
        const.Unit.ID_PEASANT: Unit.Peasant,
        const.Unit.ID_BARRACKS: Unit.Barracks,
        const.Unit.ID_TOWN_HALL: Unit.TownHall,
        const.Unit.ID_FARM: Unit.Farm
    }

    UNITS = {
        WORKER: {
            const.Race.TYPE_HUMAN: Unit.Peasant,
            const.Race.TYPE_ORC: Unit.Peasant
        },
        BASE_1: {
            const.Race.TYPE_HUMAN: Unit.TownHall,
            const.Race.TYPE_ORC: Unit.TownHall
        },
        FARM: {
            const.Race.TYPE_HUMAN: Unit.Farm,
            const.Race.TYPE_ORC: Unit.Farm
        },
        SOLDIER: {
            const.Race.TYPE_HUMAN: Unit.Footman,
            const.Race.TYPE_ORC: Unit.Footman
        },
        BARRACKS: {
            const.Race.TYPE_HUMAN: Unit.Barracks,
            const.Race.TYPE_ORC: Unit.Barracks
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





