from game import Config
from game import const
from game.state.GenericState import GenericState


class Walking(GenericState):
    id = "Walking"
    type = const.State.ID_Walking

    path_interval = .001 * Config.FRAME_MULTIPLIER


    def __init__(self, unit):
        super().__init__(unit)
        self.data = self.unit.d['s'][Walking.type]

    def init(self):
        self.data['path_timer'] = 0

    def update(self, tick):
        if self.data['path']:

            self.data['path_timer'] += tick
            if self.data['path_timer'] > self.path_interval:

                next_step = self.data['path'].pop()

                if not self.game.Map.is_walkable_tile(self.unit, *next_step):
                    self.transition()
                    return

                self.unit.set_position(*next_step)
                self.data['path_timer'] = 0
        else:
            # Nothing to do. Transition to next state
            self.transition()
            return
