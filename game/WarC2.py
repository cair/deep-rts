import json

import gc
import numpy as np
import time

from game import Config
from game.api.Event import Event
from game.graphics.GUI import GUI
from game.graphics.NoGUI import NoGUI
from game.loaders.MapLoader import MapLoader
from game.logic.Player.Player import Player
from game.logic.UnitManager import UnitManager
from game.util.ComplexEncoder import ComplexEncoder
from game.util.GameClock import GameClock


class Game:
    def __init__(self, map_name="simple", players=2, gui=True):

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
        }

        # Create game clock
        self.clock = GameClock()

        self.Map.load(self.data['tile'])  # Load tile data onto layer 2 and 3

        # Create Players
        self.players = [Player(self, x) for x in range(self.n_players)]

        # Create GUI
        self.gui = NoGUI(self) if not gui else GUI(self, self.players[0])

        self.clock.shedule(self.gui.caption, 1.0)
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

    def pause(self):
        self.paused = True

    def resume(self):
        self.paused = False

    def reset(self):
        self.gui.reset()
        self.clock.reset()
        self.units = dict()
        [p.reset() for p in self.players]


    def toJSON(self):
        return {
            'players': self.players,
            'map_name': self.map_name,
            'winner': self.winner,
            'clock': self.clock,
            'data': self.data,
            'units': self.units,
            'version': Config.VERSION
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

        Event.notify_broadcast(Event.NEW_STATE, frame)

    def dump_state(self):
        return json.dumps(self.toJSON(), cls=ComplexEncoder)

    def scheduled_save(self, tick):
        data = self.dump_state()
        if Config.SAVE_TO_FILE and Config.AI_SAVESTATE:
            with open(Config.REPORT_DIR + "state.json", "w") as f:
                f.write(data)
        self.snapshot = data

    @staticmethod
    def load(fromfile=True, raw=None):
        data = None
        if fromfile:
            try:
                with open(Config.REPORT_DIR + "state.json", "r") as f:
                    data = json.load(f)
            except FileNotFoundError as e:
                print("Could not find state-file, starting new game...")
                return Game()

        else:
            data = raw

        if data is not None:

            if not 'version' in data or data['version'] != Config.VERSION:
                v = None
                try:
                    v = data['version']
                except:
                    pass
                print("Incorrect version of the game! %s found, %s required" % (v, Config.VERSION))
                # Start plain game
                g = Game()
                return g

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
                "tile": np.array(json.loads(data['data']['tile']), dtype=np.int)
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

    def calculate_winner(self):
        alive = [p for p in self.players if not p.defeated]

        if len(alive) == 1:
            self.scheduled_save(0)                              # Save terminal game state
            self.winner = alive[0]                              # Retrieve winning player

            # Emit terminal state for defeated players
            for p in self.players:
                if p == self.winner: continue
                p.Event.notify_defeat(self.snapshot)

            # Emit terminal state for winning player
            self.winner.Event.notify_victory(self.snapshot)

            self.reset()

    def render(self, tick):
        self.gui.render(tick)
