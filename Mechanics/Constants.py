import os
import pygame


class Config:
    IS_RUNNING = True
    HAS_GUI = False

    # GameClock control.
    TICKS_PER_SECOND = 30.0
    MAX_FRAME_SKIP = 5.0

    SETTINGS = {
        "MAX": (TICKS_PER_SECOND, 0),                     # unlimited FPS
        "MAX_DOUBLE_TPS": (TICKS_PER_SECOND, TICKS_PER_SECOND * 2),  # max FPS is double TPS
        "MAX_FPS_MAP_TPS": (TICKS_PER_SECOND, TICKS_PER_SECOND),      # max FPS is TPS
        "MAX_FRPS_6TPS":(TICKS_PER_SECOND / 5, 0),                 # TPS is 6; unlimited FPS
    }




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

    Idle = 0
    Building = 1
    Harvesting = 2
    Combat = 3
    Walking = 4
    Dead = 5

    to_string = {
        Idle: "Idle",
        Building: "Building",
        Harvesting: "Harvesting",
        Combat: "Combat",
        Walking: "Walking",
        Dead: "Dead"
    }

    @staticmethod
    def state_name(state):
        return Unit.to_string[state]





class Race:
    HUMAN = 0
    ORC = 1


class Map:
    SPAWN_POINT = 9
    WALL = 0
    GRASS = 1
    WOOD = 2
    WATER = 3
    GOLD = 4
    OIL = 5
    TILE_SIZE = 32
    TILE_DATA = {

        WALL: {
            'ground': False,
            'water': False,
            'id': [51],
            'name': 'wall',
            'harvestable': False,
            'buildable': False
        },
        GRASS: {
            'ground': True,
            'water': False,
            'id': [i for i in range(238, 238 + 32)],
            'name': 'grass',
            'harvestable': False,
            'buildable': True
        },
        WOOD: {
            'ground': False,
            'water': False,
            'id': [i for i in range(102, 102 + 32)],
            'name': 'wood',
            'harvestable': True,
            'gold_yield': 0,
            'lumber_yield': 1,
            'oil_yield': 0,
            'buildable': False
        },
        WATER: {
            'ground': False,
            'water': True,
            'id': [i for i in range(300, 300 + 32)],
            'name': 'water',
            'harvestable': False,
            'buildable': False
        },
        GOLD: {
            'ground': False,
            'water': False,
            'id': [i for i in range(88, 88 + 14)],
            'name': 'wood',
            'harvestable': True,
            'gold_yield': 1,
            'lumber_yield': 0,
            'oil_yield': 0,
            'buildable': False
        },
        OIL: {
            'ground': False,
            'water': False,
            'id': [i for i in range(102, 102 + 32)],
            'name': 'oil',
            'harvestable': True,
            'gold_yield': 0,
            'lumber_yield': 0,
            'oil_yield': 1,
            'buildable': False
        }
    }
