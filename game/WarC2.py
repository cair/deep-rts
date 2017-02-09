import json

import numpy as np
import time

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
        self.snapshot = None  # Save snapshot
        # Unit Manager
        self.UnitManager = UnitManager


        self.parallell_worker = None

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
        self.clock.shedule(self.scheduled_save, Config.SAVE_FREQUENCY)
        self.clock.update(self.process, Config.UPS)  # 16
        self.clock.render(self.render, Config.FPS)  # 607

        self.winner = None  # Winner of the game
        self.paused = False

    @staticmethod
    def start(map_name, players, ParallellWorker=None):
        g = Game(map_name, players)
        g.parallell_worker = ParallellWorker()
        return g

    def set_pause(self, val):
        self.paused = val

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
            if self.paused:
                self.clock.tick_schedule()  # Continue to tick scheduled tasks
                time.sleep(.1)
                continue
            self.clock.tick()

    def process(self, tick, frame):
        for p in self.players:
            p.process(tick)

        self.gui.process()

        Event.notify_broadcast(Event.NEW_STATE, frame)

    class ComplexEncoder(json.JSONEncoder):
        def default(self, obj):
            if hasattr(obj, 'toJSON'):
                return obj.toJSON()
            elif type(obj).__name__ == 'ndarray':
                return json.dumps(obj.tolist())
            elif type(obj).__name__ == 'int64' or type(obj).__name__ == 'int32':
                return int(obj)
            else:

                return json.JSONEncoder.default(self, obj)

    def dump_state(self):
        return json.dumps(self.toJSON(), cls=Game.ComplexEncoder)

    def scheduled_save(self, tick):
        data = self.dump_state()
        if Config.SAVE_TO_FILE:
            with open(Config.REPORT_DIR + "state.json", "w") as f:
                f.write(data)
        self.snapshot = data

    @staticmethod
    def load(fromfile=True, raw=None):
        data = None
        if fromfile:
            with open(Config.REPORT_DIR + "state.json", "r") as f:
                data = json.load(f)

        else:
            data = raw

        if data is not None:
            print("Valid state file. Loading...")

            # Create new game instance
            g = Game(
                map_name=data['map_name'],
                players=len(data['players'])
            )

            # Load Game Clock
            g.clock.load(data['clock'])

            # Load game data
            g.data = {
                "unit": np.array(json.loads(data['data']['unit']), dtype=np.int),
                "unit_pid": np.array(json.loads(data['data']['unit_pid']), dtype=np.int),
                "tile": np.array(json.loads(data['data']['tile']), dtype=np.int),
                "tile_collision": np.array(json.loads(data['data']['tile_collision']), dtype=np.int)
            }

            # Load player data
            id_to_p_id = {}  # """ Key is unit id, Value is Player id"""
            id_to_player = {}  # """ Key is player id, Value is Player object"""
            for idx, p in enumerate(g.players):
                p.load(data['players'][idx])

                for unit_id in p.units:
                    id_to_p_id[unit_id] = p.id
                id_to_player[p.id] = p

            # Load unit data
            for u_id, u_v in data['units'].items():
                unit_class = UnitManager.get_class_by_id(u_v['id'])

                player_id = id_to_p_id[int(u_id)]
                player = id_to_player[player_id]

                u = unit_class(player)
                u.load(u_v)
                g.units[int(u_id)] = u
                u.unit_id = int(u_id)

        return g

    def gameover_check(self, tick):
        winner = None

        still_alive = [p for p in self.players if not p.defeated]
        if len(still_alive) == 1:
            winner = still_alive.pop()
            Config.IS_RUNNING = False
            self.winner = winner

    def render(self, tick):
        self.gui.render(tick)
