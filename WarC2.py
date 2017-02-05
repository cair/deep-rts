import threading
from threading import Thread

import numpy as np

from GUI.GUI import GUI
from GUI.NoGUI import NoGUI
from GameClock import GameClock
from Mechanics.API.Event import Event
from Mechanics.Constants import Config
from Mechanics.Map import Map
from Mechanics.Player import PlayerLogic, PlayerState
from Mechanics.Unit.UnitManager import UnitManager


class State:

    def __init__(self):
        # Units map
        self.units = {}

        # Data Matrix
        self.data = None

        # Players
        self.players = []


class Game:
    def __init__(self, map_name="simple", players=2):
        self.game_id = 1
        self.map_name = map_name
        self.n_players = players

        # Unit Manager
        self.UnitManager = UnitManager

        self.Map = Map(self)
        self.Map.preload(self.map_name)
        self.Map.load()
        self.Map.setup_spawn_locations()

        self.state = State()
        self.state.data = self.setup_data_matrix()

        self.state.players.extend([PlayerState(self, "Player %s" % x) for x in range(self.n_players)])

        # Create game clock
        self.clock = GameClock()

        # Create GUI
        self.gui = NoGUI(self) if not Config.HAS_GUI else GUI(self, self.players[0])

        self.clock.schedule(self.gui.caption, 1.0)
        self.clock.schedule(self.gameover_check, 0.001)
        self.clock.update(self.process, 50000)  # 16
        self.clock.render(self.render, 1)  # 607

        self.winner = None  # Winner of the game

    def loop(self):
        while Config.RUN_FOREVER:
            while Config.IS_RUNNING:
                self.clock.tick()

            self.reset_game()
            #if self.winner is not None:
                #print("Winner is: %s" % self.winner)
            Config.IS_RUNNING = True

    def setup_data_matrix(self):
        return {
            "unit": np.zeros((self.Map.height, self.Map.width), dtype=np.int),
            "unit_pid": np.zeros((self.Map.height, self.Map.width), dtype=np.int),
            "tile": self.Map.tiles(),
            "tile_collision": self.Map.tile_collision()
        }

    def reset_game(self):
        self.Map.setup_spawn_locations()
        self.clock.reset()
        self.state.data = self.setup_data_matrix()
        self.state.units = {}
        [p.reset() for p in self.players]
        self.game_id += 1

    def process(self, tick, frame):
        for p in self.players:
            p.process(tick)

        self.gui.process()

        if frame % Config.AI_FRAME_SKIP == 0:
            Event.notify_frame(tick)

    def gameover_check(self, tick):
        winner = None

        still_alive = [p for p in self.players if p.units and not p._surrender]
        if len(still_alive) == 1:
            winner = still_alive.pop()
            Config.IS_RUNNING = False
            self.winner = winner
        elif len(still_alive) == 0:
            Config.IS_RUNNING = False
            self.winner = winner




    def render(self, tick):
        self.gui.render(tick)
