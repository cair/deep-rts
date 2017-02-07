import numpy as np
from game.util.Util import ArrayUtil

from game.loaders.MapLoader import AdjacentMap


class MockGame:

    def __init__(self):
        self.data = {
            'tile_collision': np.zeros((30, 30), dtype=np.int),
            'unit': np.zeros((30, 30), dtype=np.int)
        }

m = AdjacentMap(MockGame(), 30, 30)
graph = np.zeros((30, 30), dtype=np.int)

for _ in range(1000):
    for x in range(29):
        for y in range(29):
            for dim in range(2):
                neighbors = m.adjacent_walkable(x, y, dim)
                neighbors = [x for x in ArrayUtil.adjacent_tiles(graph, x, y, dim) if graph[x[0]][x[1]] == 0]
