from game import Config
from game import const
from game import Constants

class Walking:
    name = "Walking"
    id = Constants.State.Walking

    def update(self, unit):
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

    def end(self, unit):
        pass

    def init(self, unit):
        self.data['path_timer'] = 0