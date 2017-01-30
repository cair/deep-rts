import os
import pygame


class Config:
    IS_RUNNING = True
    HAS_GUI = True
    INTERFACE = "Local"
    NO_FOG = True
    DEBUG = False

    FRAME_MULTIPLIER = 8

class Mouse:
    LEFT = 1
    RIGHT = 3


class Unit:
    UNIT = 0 # Peasant, Footman. etc
    STRUCTURE = 1 # Town Hall, Keep. etc

    NONE = 0
    TOWN_HALL = 1
    KEEP = 2
    CASTLE = 3
    FARM = 4
    BARRACKS = 5

    PEASANT = 20
    FOOTMAN = 21

    PEASANT_SPRITE = pygame.image.load(os.path.abspath("./data/textures/human/peasant.png"))
    FOOTMAN_SPRITE = pygame.image.load(os.path.abspath("./data/textures/human/footman.png"))
    BUILDING_SPRITESHEET = pygame.image.load(os.path.abspath("./data/textures/human/buildings.png"))

    LEFT = 0
    UP = 1
    RIGHT = 2
    DOWN = 3
    UP_LEFT = 4
    UP_RIGHT = 5
    DOWN_LEFT = 6
    DOWN_RIGHT = 7

    HARVEST_GOTO = 0
    HARVEST_WORK = 1
    HARVEST_RECALL = 2
    HARVEST_DELIVER = 3




class Race:
    HUMAN = 0
    ORC = 1


class Map:
    # Tile Ids
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
            'id': [i for i in range(238, 238 + 32)],
            'name': 'grass',
        },
        WOOD: {
            'type': HARVESTABLE,
            'id': [i for i in range(102, 102 + 32)],
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
