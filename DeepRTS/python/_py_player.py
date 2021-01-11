
class DeepRTSPlayer:

    def __init__(self, game, player):
        self.game = game  # Python game object
        self.base = player  # C++ Object

    def step(self, action):
        pass

    def reset(self):
        pass

    def _compute_state(self):
        # Compute state for the player. for example, colors must be changed accordingly
        # in the case of vector based state. we must change the player integers
        pass