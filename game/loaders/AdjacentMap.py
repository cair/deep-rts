from game.util import ArrayUtil
from game.const import Unit, Map

class AdjacentMap:

    def __init__(self, game, width, height, d=3):
        self.game = game
        self._map = {}
        self.width = width
        self.height = height
        self.dim = d

        self._generate()

    def _generate(self):

        for x in range(self.width):
            for y in range(self.height):
                for dim in range(0, self.dim):
                    key = (x, y, dim)
                    adjacent_tiles = ArrayUtil.neighbors(*key)

                    self._map[key] = adjacent_tiles

    def adjacent(self, x, y, dim):
        return self._map[(x, y, dim)]

    def adjacent_walkable(self, x, y, dim):
        potential_tiles = self.adjacent(x, y, dim)

        tiles = [t for t in potential_tiles if
                 self.game.data['tile_collision'][t[0]][t[1]] == Map.WALKABLE and
                 self.game.data['unit'][t[0]][t[1]] == Unit.NONE
                 ]

        return tiles