import os

BASE_PATH = os.path.dirname(os.path.realpath(__file__))
AI_LOCAL = 0
AI_REMOTE = 1
AI_NONE = 2

RUN_FOREVER = True # Rerun game after defeat
IS_RUNNING = True
HAS_GUI = True
INTERFACE = AI_LOCAL
NO_FOG = True
DEBUG = False
AI_FRAME_SKIP = 50 # Skip Event notification every X frames (Only State Events)
FRAME_MULTIPLIER = 8


SAVE_FREQUENCY = 1
SAVE_TO_FILE = True
FPS = 20
UPS = 32

MULTIPROCESSING_WORKERS = 10

REPORT_DIR  = os.path.join(BASE_PATH, "../reports/")


WEB_PORT = 8888