

class GameBridge:

    def __init__(self, game):
        self.g = game

    @property
    def players(self):
        return self.g.players

    @property
    def map(self):
        return self.g.map

    @property
    def units(self):
        return self.g.units
