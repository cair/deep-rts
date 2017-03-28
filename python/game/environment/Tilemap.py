import json
import os

from game import Constants
from game.environment.Tile import Tile
PATH = os.path.dirname(os.path.realpath(__file__))

class Tilemap:

    def __init__(self, map_name):
        """
        # Load map based on map_name
        :param map_name: 
        """
        self.map_name = map_name

        # Load map data
        with open(os.path.join(PATH, "../data/maps/%s" % map_name), "r") as file:
            self.map_data = json.load(file)

        # Load tile data
        with open(os.path.join(PATH, "../data/tile_properties.json"), "r") as file:
            self.tiles_data = json.load(file)

        # Define map configuration
        self.TILE_WIDTH = self.map_data["tilewidth"]
        self.TILE_HEIGHT = self.map_data["tileheight"]
        self.MAP_WIDTH = self.map_data["width"]
        self.MAP_HEIGHT = self.map_data["height"]
        self.tiles = []
        self.spawn_tiles = []

        tileset_data = self.map_data["tilesets"][0]
        map_layer = self.map_data["layers"][0]
        tile_ids = map_layer["data"]
        first_gid = tileset_data["firstgid"]

        i = 0
        for y in range(self.MAP_HEIGHT):
            for x in range(self.MAP_WIDTH):

                tile_id = tile_ids[i] - 1
                tile_data = self.tiles_data[str(tile_id + 1)]

                tile = Tile(self, x, y, self.TILE_WIDTH, self.TILE_HEIGHT)
                tile.id = i
                tile.tile_id = tile_id
                tile.name = tile_data["name"]

                tile.harvestable = tile_data["harvestable"]
                tile.walkable = tile_data["walkable"]
                tile.swimable = tile_data["swimable"]

                tile.resources = tile_data["resources"]
                tile.oil_yield = tile_data["oil_yield"]
                tile.gold_yield = tile_data["gold_yield"]
                tile.lumber_yield = tile_data["lumber_yield"]

                tile.deplete_tile = tile_data["deplete_tile"]

                if tile.name == "Spawn":
                    self.spawn_tiles.append(i)
                    tile.id = i
                    tile.tile_id = Constants.Tile.Grass - 2  # CONSTANTS GRASS TODO
                    tile.name = tile_data["name"]

                    tile.harvestable = tile_data["harvestable"]
                    tile.walkable = tile_data["walkable"]
                    tile.swimable = tile_data["swimable"]

                    tile.resources = tile_data["resources"]
                    tile.oil_yield = tile_data["oil_yield"]
                    tile.gold_yield = tile_data["gold_yield"]
                    tile.lumber_yield = tile_data["lumber_yield"]

                    tile.deplete_tile = tile_data["deplete_tile"]

                self.tiles.append(tile)
                i += 1



