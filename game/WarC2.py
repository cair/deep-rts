import json

import numpy as np

from game import Config
from game.api.Event import Event
from game.graphics.GUI import GUI
from game.graphics.NoGUI import NoGUI
from game.loaders.MapLoader import MapLoader
from game.logic.Player.Player import Player
from game.logic.UnitManager import UnitManager
from game.util.GameClock import GameClock


class Game:
    def __init__(self, map_name="simple", players=2):

        self.map_name = map_name
        self.n_players = players

        # Unit Manager
        self.UnitManager = UnitManager

        # Units map
        self.units = {}

        self.Map = MapLoader(self)
        self.Map.preload(self.map_name)

        # Data Matrix
        self.data = {
            "unit": np.zeros((self.Map.height, self.Map.width), dtype=np.int),
            "unit_pid": np.zeros((self.Map.height, self.Map.width), dtype=np.int),
            "tile": np.zeros((self.Map.height, self.Map.width), dtype=np.int),
            "tile_collision": np.zeros((self.Map.height, self.Map.width), dtype=np.int)
        }

        # Create game clock
        self.clock = GameClock()

        self.Map.load(self.data['tile'], self.data['tile_collision'])  # Load tile data onto layer 2 and 3

        # Create Players
        self.players = [Player(self, "Player %s" % x) for x in range(self.n_players)]

        # Create GUI
        self.gui = NoGUI(self) if not Config.HAS_GUI else GUI(self, self.players[0])

        self.clock.shedule(self.gui.caption, 1.0)
        self.clock.shedule(self.gameover_check, 1.0)
        self.clock.update(self.process, Config.UPS)  # 16
        self.clock.render(self.render, Config.FPS)  # 607

        self.winner = None  # Winner of the game

    @staticmethod
    def start(map_name, players):
        return Game(map_name, players)

    def toJSON(self):
        return {
            'players': self.players,
            'map_name': self.map_name,
            'winner': self.winner,
            'clock': self.clock,
            'data': self.data,
            'units': self.units
        }

    def hook(self,
             on_victory=None,
             on_defeat=None,
             on_event=None):
        """
        Hook a local AI
        :return:
        """
        p = self.players[0]
        p.Event.on_event(on_event)
        p.Event.on_defeat(on_defeat)
        p.Event.on_victory(on_victory)
        return p.id, p.Event.Action


    def loop(self):
        while Config.IS_RUNNING:
            self.clock.tick()

    def process(self, tick, frame):
        for p in self.players:
            p.process(tick)

        self.gui.process()

        Event.notify_broadcast(Event.NEW_STATE, frame)

    class ComplexEncoder(json.JSONEncoder):
        def default(self, obj):
            if hasattr(obj,'toJSON'):
                return obj.toJSON()
            elif type(obj).__name__ == 'ndarray':
                return json.dumps(obj.tolist())
            else:
                return json.JSONEncoder.default(self, obj)

    def save(self):

        with open("./test.json", "w") as f:
            f.write(json.dumps(self.toJSON(), cls=Game.ComplexEncoder, sort_keys=True, indent=4))


    def gameover_check(self, tick):

        #self.save()

        winner = None

        still_alive = [p for p in self.players if not p.defeated]
        if len(still_alive) == 1:
            winner = still_alive.pop()
            Config.IS_RUNNING = False
            self.winner = winner




    def render(self, tick):
        self.gui.render(tick)
