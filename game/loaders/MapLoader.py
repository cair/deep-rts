import logging
import os
from game.const import Unit, Map
import numpy as np

dir_path = os.path.dirname(os.path.realpath(__file__))


class MapLoader:
    preloaded = False
    AdjacentMap = None
    spawn_tiles = []
    height = None
    width = None
    tiles = []
    TILES_THEME = "summer"

    @staticmethod
    def preload(map_name):
        # Parse raw map
        raw_data = None
        logging.debug("Loading map %s" % map_name)
        with open(os.path.join(dir_path, '../data/maps/', map_name + ".map")) as f:
            raw_data = [[int(digit) for digit in line.split()] for line in f]

        MapLoader.height = len(raw_data[0])
        MapLoader.width = len(raw_data)

        MapLoader.tiles = np.zeros((MapLoader.height, MapLoader.width), dtype=np.int8)

        for y, val in enumerate(raw_data):
            for x, tile_id in enumerate(val):

                # If spawn point, add to spawn_point list
                if tile_id == Map.SPAWN_POINT:
                    MapLoader.spawn_tiles.append((x, y))
                    tile_id = Map.GRASS

                MapLoader.tiles[x][y] = tile_id

        logging.debug("Loaded %s, a %sX%s sized map!" % (map_name, MapLoader.height, MapLoader.width))
        MapLoader.preloaded = True

    @staticmethod
    def get_spawn_tile(player_id):
        return MapLoader.spawn_tiles[player_id]

    @staticmethod
    def is_harvestable_tile(unit, x, y):
        tile_id = MapLoader.tiles[x][y]
        tile = Map.TILE_DATA[tile_id]

        return tile['type'] == Map.HARVESTABLE

    @staticmethod
    def is_walkable_tile(unit, x, y):
        tile_walkable = MapLoader.tiles[x][y] == Map.WALKABLE
        unit_walkable = unit.game.data['unit'][x, y] == Unit.NONE

        return tile_walkable and unit_walkable

    @staticmethod
    def is_attackable(unit, x, y):
        unit_player = unit.game.data['unit_pid'][x, y]
        unit_data = unit.game.data['unit'][x, y]

        return unit_data != Unit.NONE and unit_player != unit.player.id

    @staticmethod
    def get_tile(x, y):
        tile_id = MapLoader.tiles[x][y]
        tile = Map.TILE_DATA[tile_id]
        return tile

