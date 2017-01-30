import numpy as np

from GUI.GUI import GUI
from GUI.NoGUI import NoGUI
from GameClock import GameClock
from Mechanics.API.Event import Event
from Mechanics.Constants import Config
from Mechanics.Map import Map
from Mechanics.Player import Player
from Mechanics.Unit.UnitManager import UnitManager


class Game(object):
    def __init__(self, map_name="simple", players=2):

        # Unit Manager
        self.UnitManager = UnitManager

        # Units map
        self.units = {}

        self.Map = Map(self)
        self.Map.preload(map_name)

        # Data Matrix
        self.data = {
            "unit": np.zeros((self.Map.height, self.Map.width), dtype=np.int),
            "unit_pid": np.zeros((self.Map.height, self.Map.width), dtype=np.int),
            "tile": np.zeros((self.Map.height, self.Map.width), dtype=np.int),
            "tile_collision": np.zeros((self.Map.height, self.Map.width), dtype=np.int)
        }

        self.Map.load(self.data['tile'], self.data['tile_collision'])  # Load tile data onto layer 2 and 3

        # Create Players
        self.players = [Player(self, "Player %s" % x) for x in range(players)]

        # Create GUI
        self.gui = NoGUI(self) if not Config.HAS_GUI else GUI(self, self.players[0])

        # Create game clock
        self.clock = GameClock()
        self.clock.shedule(self.gui.caption, 1.0)
        self.clock.update(self.process, 8)  # 16
        self.clock.render(self.render, 60)  # 60

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
        Event.notify_broadcast(Event.New_State, frame)

    def render(self, tick):
        self.gui.render(tick)
