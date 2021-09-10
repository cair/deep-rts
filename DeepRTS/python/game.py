from DeepRTS import Engine, Constants
from DeepRTS.python import GUI
from DeepRTS.python import Config
from DeepRTS.python import DeepRTSPlayer

import numpy as np
import random
import os
import argparse
import gym



dir_path = os.path.dirname(os.path.realpath(__file__))


class Game(Engine.Game):

    def __init__(self,
                 map_name,
                 n_players=2,
                 engine_config=Engine.Config.defaults(),
                 gui_config=None,
                 tile_size=32,
                 terminal_signal=False
                 ):
        # This sets working directory, so that the C++ can load files correctly (dir_path not directly accessible in
        # c++)
        os.chdir(dir_path)

        # Override map
        try:
            # This sometimmes fails under ray
            parser = argparse.ArgumentParser(description='Process some integers.')
            parser.add_argument('--map', action="store", dest="map", type=str)
            args = parser.parse_args()

            if args.map is not None:
                map_name = args.map
        except:
            pass

        # TODO
        if engine_config:
            engine_config.set_terminal_signal(terminal_signal)
        # Disable terminal signal
        engine_config.set_terminal_signal(terminal_signal)

        # Call C++ constructor
        super(Game, self).__init__(map_name, engine_config)

        # Event listeners
        self._listeners = {
            "on_tile_change": []
        }

        self._render_every = 1
        self._view_every = 1
        self._capture_every = 1

        self.gui = GUI(self, tile_size=tile_size, config=gui_config if isinstance(gui_config, Config) else Config())
        self._py_players = []  # Keep reference of the py object
        for i in range(n_players):
            player = DeepRTSPlayer(self)
            self.insert_player(player)
            self._py_players.append(player)

        # Select first player as default
        self.set_player(self.players[0])

        # Define the action space
        self.action_space = LimitedDiscrete(Constants.action_min, Constants.action_max)

        # Define the observation space, here we assume that max is 255 (image) # TODO
        self.observation_space = gym.spaces.Box(
            0,
            255,
            shape=self.get_state().shape, dtype=np.float32)

        self.start()

    @property
    def players(self):
        return self._py_players

    @staticmethod
    def sample_action(self):
        return int(Engine.Constants.action_max * random.random()) + Engine.Constants.action_min

    def update(self):
        self.tick()

        if self.gui.config.input:
            self.event()

        super().update()

        self.caption()

        if self.gui.config.render:
            self.render()

        if self.gui.config.view:
            self.view()

    def _render(self):
        if self.get_ticks() % self._render_every == 0:
            self.gui.render()

    def view(self):
        if self.get_ticks() % self._view_every == 0:
            self.gui.view()

    def event(self):
        self.gui.event()

    def capture(self):
        if self.get_ticks() % self._capture_every == 0:
            return self.gui.capture()
        return None

    def get_state(self, image=False, copy=True):
        if image:
            return self.gui.capture(copy=copy)
        else:
            return np.array(self.state, copy=copy)

    def _caption(self):
        pass

    def _on_unit_create(self, unit):
        pass

    def _on_unit_destroy(self, unit):
        pass

    def _on_episode_start(self):
        pass
        # for tile in self.tilemap.tiles:
        #    self.gui.gui_tiles.set_tile(tile.x, tile.y, tile.get_type_id())

    def _on_episode_end(self):
        pass

    def _on_tile_deplete(self, tile):
        # TODO
        pass
        # self.gui.gui_tiles.set_tile(tile.x, tile.y, tile.get_type_id())

    def _on_tile_change(self, tile):
        self.gui.on_tile_change(tile)

    def set_render_frequency(self, interval):
        self._render_every = interval

    def set_player(self, player: DeepRTSPlayer):
        self.set_selected_player(player)

    def set_view_every(self, n):
        self._view_every = n

    def set_capture_every(self, n):
        self._capture_every = n