import multiprocessing
import os
#from ai.algorithms import Hardcode
#from ai.algorithms.GreedyMCTS.old.GreedyMCTS import GreedyMCTS
#from ai.algorithms.RandomAction import RandomAction
from game.util import Version

try:
    import json
    LIBRARY_JSON = json
except:
    import json
    LIBRARY_JSON = json
    print("ujson could not be found. This is a huge performance boost! pip install ujson")



BASE_PATH = os.path.dirname(os.path.realpath(__file__))
AI_LOCAL = 0
AI_REMOTE = 1
AI_NONE = 2

RUN_FOREVER = True # Rerun game after defeat
IS_RUNNING = True

INTERFACE = AI_LOCAL
NO_FOG = True
DEBUG = False
AI_FRAME_SKIP = 50 # Skip Event notification every X frames (Only State Events)
FRAME_MULTIPLIER = 8


GUI_ON = True
GUI_NOGUI_CAPTION = True

GAME_MAP = "simple"
GAME_LUMBER_MODIFIER = 1
GAME_GOLD_MODIFIER = 1.25
GAME_OIL_MODIFIER = 0

MECHANICS_HARVEST_FOREVER = False            # This options make workers harvest resources indefinitely
MECHANICS_TOWN_HALL_ON_START = True          # This option makes the worker automatically start building town-hall


#AI_ALGORITHMS = [GreedyMCTS, RandomAction, Hardcode.Hardcode_Simple1] # TODO circular
AI_SAVESTATE = False


SAVE_FREQUENCY = 1
SAVE_TO_FILE = True
FPS = 5
UPS = 20000000

MULTIPROCESSING_WORKERS = 0 #multiprocessing.cpu_count()
MULTIPROCESSING_GUI = False

REPORT_DIR = os.path.join(BASE_PATH, "../reports/")
WEB_PORT = 8888


VERSION = Version.get_version()

