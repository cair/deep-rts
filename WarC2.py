from Event import Event
from GameClock import GameClock
from Mechanics.Map import Map
from Mechanics.Player import Player
from GUI.GUI import GUI
from GUI.NoGUI import NoGUI
from Mechanics.Constants import Config
from Mechanics.Unit.UnitManager import UnitManager
import numpy as np

class Game(object):



    def __init__(self, map_name="simple", players=2):
        Map.preload(map_name)

        # Unit Manager
        self.UnitManager = UnitManager
        self.Map = Map

        # Units map
        self.units = {}

        # Data Matrix
        self.data = {
            "unit": np.zeros((Map.height, Map.width), dtype=np.int),
            "unit_pid": np.zeros((Map.height, Map.width), dtype=np.int),
            "tile": np.zeros((Map.height, Map.width), dtype=np.int),
            "tile_collision": np.zeros((Map.height, Map.width), dtype=np.int)
        }

        Map.load(self.data['tile'], self.data['tile_collision'])  # Load tile data onto layer 2 and 3

        # Create Players
        self.players = [Player(self, "Player %s" % x) for x in range(players)]

        # Create GUI
        self.gui = NoGUI(self) if not Config.HAS_GUI else GUI(self, self.players[0])

        # Create game clock
        self.clock = GameClock()
        self.clock.shedule(self.gui.caption, 1.0)
        self.clock.update(self.process, 100000000000)  # 16
        self.clock.render(self.render, 1)  # 60

        self.loop()

    def loop(self):
        while Config.IS_RUNNING:
            self.clock.tick()

    def process(self, tick, frame):

        for p in self.players:
            p.process(tick)

            # TODO Game over (Does not scale with 2+ players)
            if p.is_defeated():
                Config.IS_RUNNING = False



        self.gui.process()
        Event.notify(Event.New_State, frame)

    def render(self, tick):
        self.gui.render(tick)

