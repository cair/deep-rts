from game.state.GenericState import GenericState
from game.const.State import ID_Idle

class Idle(GenericState):
    id = "Idle"
    type = ID_Idle

    def __init__(self, unit):
        super().__init__(unit)

    def update(self, tick):
        pass


# Register to Generic State
GenericState.registry[Idle.type] = Idle