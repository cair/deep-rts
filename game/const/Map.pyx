SPAWN_POINT = 9
WALL = 0
GRASS = 1
WOOD = 2
WATER = 3
GOLD = 4
OIL = 5

# Tile Types
BLOCKED = 0
WALKABLE = 1
HARVESTABLE = 2
SWIMABLE = 3

TILE_SIZE = 32
TILE_DATA = {

    WALL: {
        'type': BLOCKED,
        'id': [51],
        'name': 'wall',
    },
    GRASS: {
        'type': WALKABLE,
        'id': [250], #[i for i in range(238, 238 + 32)],
        'name': 'grass',
    },
    WOOD: {
        'type': HARVESTABLE,
        'id': [105], #[i for i in range(102, 102 + 32)],
        'name': 'wood',
        'gold_yield': 0,
        'lumber_yield': 1,
        'oil_yield': 0,
    },
    WATER: {
        'type': BLOCKED, # todo SWIMABLE?
        'id': [i for i in range(300, 300 + 32)],
        'name': 'water',
    },
    GOLD: {
        'type': HARVESTABLE,
        'id': [i for i in range(88, 88 + 14)],
        'name': 'wood',
        'gold_yield': 1,
        'lumber_yield': 0,
        'oil_yield': 0,
    },
    OIL: {
        'type': HARVESTABLE,
        'id': [i for i in range(102, 102 + 32)],
        'name': 'oil',
        'gold_yield': 0,
        'lumber_yield': 0,
        'oil_yield': 1,
    }
}