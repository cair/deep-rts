import logging
import os
from game.const import Unit, Map
from game.loaders.AdjacentMap import AdjacentMap

dir_path = os.path.dirname(os.path.realpath(__file__))


class MapLoader:

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
        with open(os.path.join(dir_path, '../data/maps/', map_name + ".map")) as f:
            self.raw_data = [[int(digit) for digit in line.split()] for line in f]

        self.height, self.width = len(self.raw_data[0]), len(self.raw_data)
        self.AdjacentMap = AdjacentMap(self.game, self.height, self.width)

        logging.debug("Loaded %s, a %sX%s sized map!" % (map_name, self.height, self.width))

    def load(self, tiles):

        for y, val in enumerate(self.raw_data):
            for x, tile_id in enumerate(val):

                # If spawn point, add to spawn_point list
                if tile_id == Map.SPAWN_POINT:
                    self.spawn_tiles.append((x, y))
                    tile_id = Map.GRASS


                tiles[x][y] = tile_id

    def get_spawn_tile(self, player_id):
        return self.spawn_tiles[player_id]

    @staticmethod
    def is_harvestable_tile(unit, x, y):
        tile_id = unit.game.data['tile'][x][y]
        tile = Map.TILE_DATA[tile_id]

        return tile['type'] == Map.HARVESTABLE

    @staticmethod
    def is_walkable_tile(unit, x, y):
        tile_walkable = unit.game.data['tile'][x][y] == Map.WALKABLE
        unit_walkable = unit.game.data['unit'][x][y] == Unit.NONE

        return tile_walkable and unit_walkable

    @staticmethod
    def is_attackable(unit, x, y):
        unit_player = unit.game.data['unit_pid'][x][y]
        unit_data = unit.game.data['unit'][x][y]

        return unit_data != Unit.NONE and unit_player != unit.player.id

    def get_unit(self, x, y):
        return self.game.units[self.game.data['unit'][x][y]]

    def get_tile(self, x, y):
        tile_id = self.game.data['tile'][x][y]
        tile = Map.TILE_DATA[tile_id]
        return tile

