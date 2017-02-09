from game import Config


class NoGUI:
    def __init__(self, game):
        self.game = game

    def render(self, dt):
        pass

    def process(self):
        pass

    def caption(self, dt):
        print(
            ' '.join(('Loop=GameClock Tab:[TPS=%d MaxFPS=%d]',
                      'Runtime:[FPS=%d UPS=%d MUL=x%d]')) % (
                self.game.clock.max_ups,
                self.game.clock.max_fps,
                self.game.clock.fps,
                self.game.clock.ups,
                (self.game.clock.ups / Config.FRAME_MULTIPLIER)))
