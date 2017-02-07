from game import Config
from game.state.GenericState import GenericState
from game.const.State import ID_Walking
from game.loaders.MapLoader import MapLoader
from game.state.Idle import Idle


class Walking(GenericState):
    id = "Walking"
    type = ID_Walking
    default = Idle

    path_interval = .001 * Config.FRAME_MULTIPLIER
    path_timer = 0
    path = []
    path_goal = None

    def __init__(self, unit, attributes={}):
        super().__init__(unit, attributes)

    def toJSON2(self):
        return {
            'path_timer': self.path_timer,
            'path': self.path,
            'path_goal': self.path_goal
        }

    def update(self, tick):
        if self.path:

            self.path_timer += tick
            if self.path_timer > self.path_interval:

                next_step = self.path.pop()

                if not MapLoader.is_walkable_tile(self.unit, *next_step):
                    self.transition()
                    return

                self.unit.set_position(*next_step)
                self.path_timer = 0
        else:
            # Nothing to do. Transition to next state
            self.transition()
            return
