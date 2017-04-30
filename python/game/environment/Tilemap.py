from game.loaders.ResourceLoader import ResourceLoader
from game.const import Map
class Tilemap:


    def __init__(self, map_name, game):

        mapJSON, tileJSON = ResourceLoader.loadMapJSON()

        tileset_data = mapJSON["tilesets"][0]
        map_layer = mapJSON["layers"][0]
        tileIDs = map_layer["data"]

        mapWidth = mapJSON["width"]
        mapHeight = mapJSON["height"]
        tWidth = tileset_data["tilewidth"]
        tHeight = tileset_data["tileheight"]
        tFirstGid = tileset_data["firstgid"]

        self.TILE_WIDTH = tWidth
        self.TILE_HEIGHT = tHeight
        self.MAP_WIDTH = mapWidth
        self.MAP_HEIGHT = mapHeight
        self.tileTextureDimension = 32
        self.tileWorldDimension = 32
        self.tiles = []
        self.spawnTiles = []

        c = 0
        for y in range(self.MAP_HEIGHT):
            for x in range(self.MAP_WIDTH):

                tId = tileIDs[c] - 1
                tileData = tileJSON[str(tId + 1)]

                tile = Tile(
                    x,
                    y,
                    self.TILE_WIDTH,
                    self.TILE_HEIGHT,
                    self, tileData["walkable"],
                    tileData["harvestable"],
                    tileData["resources"]
                )

                self.tiles.append(tile)
                tile.tileID = tId
                tile.id = c
                tile.name = tileData["name"]
                tile.harvestable = tileData["harvestable"]
                tile.walkable = tileData["walkable"]
                tile.swimable = tileData["swimable"]
                tile.oilYield =  tileData["oil_yield"]
                tile.lumberYield = tileData["lumber_yield"]
                tile.goldYield = tileData["gold_yield"]
                tile.setResources(tileData["resources"])
                tile.depleteTile = tileData["deplete_tile"]

                if tile.name == "Spawn":
                    self.spawnTiles.push_back(c)
                    # Replace spawn data with grass
                    tId = Map.ID_GRASS
                    tileData = tileJSON[str(tId - 1)]

                    tId -= 2  # TODO some fucked up shit goin on here
                    tile.id = c
                    tile.tileID = tId
                    tile.name = tileData["name"]
                    tile.harvestable = tileData["harvestable"]
                    tile.walkable = tileData["walkable"]
                    tile.swimable = tileData["swimable"]
                    tile.oilYield =  tileData["oil_yield"]
                    tile.lumberYield = tileData["lumber_yield"]
                    tile.goldYield = tileData["gold_yield"]
                    tile.setResources(tileData["resources"])
                    tile.depleteTile = tileData["deplete_tile"]
                    
                c += 1



