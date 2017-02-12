from os.path import join
import pygame
from game import Config

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

PEASANT_SPRITE = pygame.image.load(join(Config.BASE_PATH, "./data/textures/human/peasant.png"))
FOOTMAN_SPRITE = pygame.image.load(join(Config.BASE_PATH, "./data/textures/human/footman.png"))
BUILDING_SPRITESHEET = pygame.image.load(join(Config.BASE_PATH, "./data/textures/human/buildings.png"))

LEFT = 0
UP = 1
RIGHT = 2
DOWN = 3
UP_LEFT = 4
UP_RIGHT = 5
DOWN_LEFT = 6
DOWN_RIGHT = 7
