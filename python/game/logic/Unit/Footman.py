from game import const
from game import Config
from game.logic.Unit.Unit import Unit


class Footman(Unit):

    # Identifier
    id = id = const.Unit.ID_FOOTMAN
    structure = const.Unit.TYPE_UNIT
    military = True

    # Tile Data
    width = 1
    height = 1

    # Economics
    cost_lumber = 0
    cost_gold = 600
    build_duration = 60 * Config.FRAME_MULTIPLIER

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

    def __init__(self, player, init=True):
        super().__init__(player, init)
        self.player = player

        self.buildable = [
        ]




