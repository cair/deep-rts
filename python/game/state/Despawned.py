from game.state.GenericState import GenericState
from game.const.State import ID_Despawned
from game.state.Idle import Idle


class Despawned(GenericState):
    id = "Despawned"
    type = ID_Despawned
    default = Idle

    def __init__(self, unit):
        super().__init__(unit)
        self.done = False

    def update(self, dt):
        if not self.done:
            self.unit.despawn()

