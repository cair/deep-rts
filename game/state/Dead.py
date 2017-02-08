from game.state.GenericState import GenericState
from game.const.State import ID_Dead
from game.state.Idle import Idle


class Dead(GenericState):
    id = "Dead"
    type = ID_Dead
    default = Idle

    def __init__(self, unit, attributes={}):
        super().__init__(unit, attributes)

    def init(self):
        self.unit.despawn()
        self.unit.remove_from_game()

    def update(self, tick):
        pass

# Register to Generic State
GenericState.registry[Dead.type] = Dead