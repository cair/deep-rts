

class Tile:

    def __init__(self, tilemap, x, y, w, h):
        self.id = None
        self.tile_id = None

        self.name = None

        self.tilemap = tilemap
        self.x = x
        self.y = y
        self.width = w
        self.height = h

        self.walkable = False
        self.harvestable = False
        self.swimable = False

        self.resources = 0
        self.oil_yield = 0
        self.lumber_yield = 0
        self.gold_yield = 0

        self.deplete_tile = None