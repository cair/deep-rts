import os
import numpy as np
from Mechanics.Constants import Map as MapC
from Mechanics.Constants import Unit as UnitC
from Mechanics.Util import ArrayUtil
import logging


class Map:

    TILES_THEME = "summer"
    width = None
    height = None
    raw_data = None
    spawn_tiles = []

    @staticmethod
    def preload(map_name):
        # Parse raw map
        logging.debug("Loading map %s" % map_name)
        with open(os.path.join('./data/maps/', map_name + ".map")) as f:
            Map.raw_data = [[int(digit) for digit in line.split()] for line in f]

        Map.height, Map.width = len(Map.raw_data[0]), len(Map.raw_data)
        logging.debug("Loaded %s, a %sX%s sized map!" % (map_name, Map.height, Map.width))

    @staticmethod
    def load(tiles, tile_collision):

        for y, val in enumerate(Map.raw_data):
            for x, tile_id in enumerate(val):

                # If spawn point, add to spawn_point list
                if tile_id == MapC.SPAWN_POINT:
                    Map.spawn_tiles.append((x, y))
                    tile_id = MapC.GRASS

                tile = MapC.TILE_DATA[tile_id]

                tiles[x][y] = tile_id
                tile_collision[x][y] = tile['type']

    @staticmethod
    def get_spawn_tile():
        return Map.spawn_tiles.pop(0)

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

    @staticmethod
    def buildable_here(unit, x, y, d):
        tiles = unit.game.data['tile']

        neighbors = ArrayUtil.neighbors(tiles, x, y, d)
        common = list(set(neighbors).intersection(set(Map.free_tiles(unit))))
        return len(common) == len(neighbors)

    @staticmethod
    def get_tile(game, x, y):
        tile_id = game.data['tile'][x][y]
        tile = MapC.TILE_DATA[tile_id]
        return tile

