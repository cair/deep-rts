from os.path import join
import pygame
from game import Config

TYPE_UNIT = "0"
TYPE_STRUCTURE = "1"

ID_NONE = "0"
ID_TOWN_HALL = "1"
ID_KEEP = "2"
ID_CASTLE = "3"
ID_FARM = "4"
ID_BARRACKS = "5"

ID_PEASANT = "20"
ID_FOOTMAN = "21"

DIRECTION_LEFT = "0"
DIRECTION_UP = "1"
DIRECTION_RIGHT = "2"
DIRECTION_DOWN = "3"
DIRECTION_UP_LEFT = "4"
DIRECTION_UP_RIGHT = "5"
DIRECTION_DOWN_LEFT = "6"
DIRECTION_DOWN_RIGHT = "7"


GUI_PEASANT_SPRITE = pygame.image.load(join(Config.BASE_PATH, "./data/textures/human/peasant.png"))
GUI_FOOTMAN_SPRITE = pygame.image.load(join(Config.BASE_PATH, "./data/textures/human/footman.png"))
GUI_BUILDING_SPRITESHEET = pygame.image.load(join(Config.BASE_PATH, "./data/textures/human/buildings.png"))


