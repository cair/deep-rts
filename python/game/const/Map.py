

ID_SPAWN_POINT = 9
ID_WALL = 0
ID_GRASS = 1
ID_WOOD = 2
ID_WATER = 3
ID_GOLD = 4
ID_OIL = 5

# Tile Types
TYPE_BLOCKED = 0
TYPE_WALKABLE = 1
TYPE_HARVESTABLE = 2
TYPE_SWIMABLE = 3

TILE_SIZE = 32
TILE_DATA = {

    ID_WALL: {
        'type': TYPE_BLOCKED,
        'id': [51],
        'name': 'wall',
    },
    ID_GRASS: {
        'type': TYPE_WALKABLE,
        'id': [250], #[i for i in range(238, 238 + 32)],
        'name': 'grass',
    },
    ID_WOOD: {
        'type': TYPE_HARVESTABLE,
        'id': [105], #[i for i in range(102, 102 + 32)],
        'name': 'wood',
        'gold_yield': 0,
        'lumber_yield': 1,
        'oil_yield': 0,
    },
    ID_WATER: {
        'type': TYPE_BLOCKED, # todo SWIMABLE?
        'id': [i for i in range(300, 300 + 32)],
        'name': 'water',
    },
    ID_GOLD: {
        'type': TYPE_HARVESTABLE,
        'id': [i for i in range(88, 88 + 14)],
        'name': 'wood',
        'gold_yield': 1,
        'lumber_yield': 0,
        'oil_yield': 0,
    },
    ID_OIL: {
        'type': TYPE_HARVESTABLE,
        'id': [i for i in range(102, 102 + 32)],
        'name': 'oil',
        'gold_yield': 0,
        'lumber_yield': 0,
        'oil_yield': 1,
    }
}