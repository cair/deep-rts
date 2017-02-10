import multiprocessing
import os

from ai.algorithms import Hardcode
from ai.algorithms.RandomAction import RandomAction
from game.util import Version

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


GUI_ON = False
GUI_NOGUI_CAPTION = False



AI_ALGORITHMS = [RandomAction, Hardcode.Hardcode_Simple1]
AI_SAVESTATE = False

SAVE_FREQUENCY = 1
SAVE_TO_FILE = True
FPS = 20
UPS = 20000000

MULTIPROCESSING_WORKERS = multiprocessing.cpu_count() - 6

REPORT_DIR = os.path.join(BASE_PATH, "../reports/")
WEB_PORT = 8888


VERSION = Version.get_version()

