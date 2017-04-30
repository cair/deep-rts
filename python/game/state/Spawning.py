from game.state.GenericState import GenericState
from game.const.State import ID_Spawning
from game.state.Idle import Idle


class Spawning(GenericState):
    id = "Spawning"
    type = ID_Spawning
    default = Idle

    def __init__(self, unit):
        super().__init__(unit)
        self.data = unit.d['s'][Spawning.type]

    def update(self, tick):
        pass

