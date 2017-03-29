from game import Config
from sfml

class GUI:
    def __init__(self, game):
        self.game = game

    def render(self):
        pass

    def update(self):
        pass

    def reset(self):
        pass

    def caption(self):
        if Config.GUI_NOGUI_CAPTION:
            print(
                ' '.join(('Loop=GameClock Tab:[TPS=%d MaxFPS=%d]',
                          'Runtime:[FPS=%d UPS=%d MUL=x%d]')) % (
                    self.game.clock.max_ups,
                    self.game.clock.max_fps,
                    self.game.clock.fps,
                    self.game.clock.ups,
                    (self.game.clock.ups / Config.FRAME_MULTIPLIER)))
