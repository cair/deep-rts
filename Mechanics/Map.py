import os
import numpy as np
from Mechanics.Constants import Map as MapC
from Mechanics.Constants import Unit as UnitC
from Mechanics.Util import ArrayUtil
import logging

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
                for dim in range(1, self.dim):
                    key = (x, y, dim)
                    adjacent_tiles = ArrayUtil.neighbors(*key)

                    self._map[key] = adjacent_tiles

    def adjacent(self, x, y, dim):
        return self._map[(x, y, dim)]

    def adjacent_walkable(self, x, y, dim):
        potential_tiles = self.adjacent(x, y, dim)

        tiles = [t for t in potential_tiles if
                 self.game.data['tile_collision'][t[0]][t[1]] == MapC.WALKABLE and
                 self.game.data['unit'][t[0]][t[1]] == UnitC.NONE
                 ]

        return tiles


class Map:

    def __init__(self, game):
        self.TILES_THEME = "summer"
        self.width = None
        self.height = None
        self.raw_data = None
        self.spawn_tiles = []
        self.AdjacentMap = None
        self.game = game

    def preload(self, map_name):
        # Parse raw map
        logging.debug("Loading map %s" % map_name)
        with open(os.path.join('./data/maps/', map_name + ".map")) as f:
            self.raw_data = [[int(digit) for digit in line.split()] for line in f]

        self.height, self.width = len(self.raw_data[0]), len(self.raw_data)
        self.AdjacentMap = AdjacentMap(self.game, self.height, self.width)

        logging.debug("Loaded %s, a %sX%s sized map!" % (map_name, self.height, self.width))

    def load(self, tiles, tile_collision):

        for y, val in enumerate(self.raw_data):
            for x, tile_id in enumerate(val):

                # If spawn point, add to spawn_point list
                if tile_id == MapC.SPAWN_POINT:
                    self.spawn_tiles.append((x, y))
                    tile_id = MapC.GRASS

                tile = MapC.TILE_DATA[tile_id]

                tiles[x][y] = tile_id
                tile_collision[x][y] = tile['type']

    def get_spawn_tile(self):
        return self.spawn_tiles.pop(0)

    @staticmethod
    def is_harvestable_tile(unit, x, y):
        tile_id = unit.game.data['tile'][x][y]
        tile = MapC.TILE_DATA[tile_id]

        return tile['type'] == MapC.HARVESTABLE

    @staticmethod
    def is_walkable_tile(unit, x, y):
        tile_walkable = unit.game.data['tile'][x][y] == MapC.WALKABLE
        unit_walkable = unit.game.data['unit'][x][y] == UnitC.NONE

        return tile_walkable and unit_walkable

    @staticmethod
    def is_attackable(unit, x, y):
        unit_player = unit.game.data['unit_pid'][x][y]
        unit_data = unit.game.data['unit'][x][y]

        return unit_data != UnitC.NONE and unit_player != unit.player.id

    @staticmethod
    def free_tiles(unit):
        """

        :return: all tiles that units can be placed on
        """

        tiles = unit.game.data['tile']
        units = unit.game.data['unit']

        # Environment tiles
        env_tiles = np.where(tiles == MapC.GRASS)
        env_tiles = set(zip(*env_tiles))

        # Unit tiles
        unit_tiles = np.where(units == UnitC.NONE)
        unit_tiles = set(zip(*unit_tiles))

        common = list(env_tiles.intersection(unit_tiles))

        return common

    @staticmethod
    def get_unit(unit, x, y):
        return unit.game.units[unit.game.data['unit_pid'][x][y]]

    def walkable_neighbor_tiles(self, x, y, d):
        # All possible tiles
        neighbors = ArrayUtil.neighbors(self.tiles, x, y, d + 1)
        neighbors.append((x, y))

        # Filter occupied tiles
        neighbors = [(x, y) for x, y in neighbors if self.game.unit_map[x][y] == UnitC.NONE]

        # Filter non walkable tiles
        neighbors = [(x, y) for x, y in neighbors if self.tiles[x][y] == MapC.GRASS]

        return neighbors

    def buildable_here(self, unit, x, y, d):

        neighbors = ArrayUtil.neighbors(x, y, d)
        common = list(set(neighbors).intersection(set(self.free_tiles(unit))))
        return len(common) == len(neighbors)

    def get_tile(self, x, y):
        tile_id = self.game.data['tile'][x][y]
        tile = MapC.TILE_DATA[tile_id]
        return tile

