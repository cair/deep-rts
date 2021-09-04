from DeepRTS import Player


class DeepRTSPlayer(Player):

    def __init__(self, game):
        self.game_instance = game
        super(DeepRTSPlayer, self).__init__(game, len(game.players))

    def step(self, action):
        pass

    def get_state(self):

        return self.game_instance.get_state()

    def reset(self):
        pass

    def _compute_state(self):
        # Compute state for the player. for example, colors must be changed accordingly
        # in the case of vector based state. we must change the player integers
        pass