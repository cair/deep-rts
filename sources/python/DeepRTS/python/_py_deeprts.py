from DeepRTS import Engine
from DeepRTS.python import GUI
from DeepRTS.python import Config
import numpy as np
import random


class Game(Engine.Game):

    def __init__(self, map_name, n_players=2, engine_config=None, gui_config=None, tile_size=32):
        super_args = (map_name, engine_config) if engine_config else (map_name, )
        super(Game, self).__init__(*super_args)

        self.gui_config = gui_config if isinstance(gui_config, Config) else Config()

        self._listeners = {
            "on_tile_change": []
        }

        # Create players
        for i in range(n_players):
            self.add_player()

        self._render_every = 1
        self._view_every = 1
        self._capture_every = 1

        self.gui = GUI(self, tile_size=tile_size)

        # Must have this to trigger tile refresh (full)
        # This is because the onTileChange callback is added in self.gui, which is AFTER tiles are created
        # Also the game resets once before the gui is created
        # This should have minimal impact on performance
        self.init()

        # Select first player as default
        self.set_player(self.players[0])

    def sample_action(self):
        return int(Engine.Constants.action_max * random.random()) + Engine.Constants.action_min
        # Slow
        #return random.randint(Engine.Constants.action_min, Engine.Constants.action_max)

    def render_every(self, interval):
        self._render_every = interval

    def tick(self):
        super().tick()

    def update(self):
        self.tick()

        if self.gui_config.input:
            self.event()

        super().update()

        self.caption()

        if self.gui_config.render:
            self.render()

        if self.gui_config.view:
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

    def set_player(self, player):
        self.set_selected_player(player)

    def get_state(self, image=False, copy=False):
        return self.gui.capture() if image else np.array(self.state, copy=copy)

    def view_every(self, n):
        self._view_every = n

    def capture_every(self, n):
        self._capture_every = n

    def _caption(self):
        pass
        """self.gui.set_caption("DeepRTS v2.0 - [FPS=%d UPS=%d MUL=x%d]" %
                             (
                                 self.get_fps(),
                                 self.get_ups(),
                                 self.get_ups() / self.get_ticks_modifier()
                             ))"""

    def _on_unit_create(self, unit):
        pass

    def _on_unit_destroy(self, unit):
        pass

    def _on_episode_start(self):
        pass
        #for tile in self.tilemap.tiles:
        #    self.gui.gui_tiles.set_tile(tile.x, tile.y, tile.get_type_id())

    def _on_episode_end(self):
        pass

    def _on_tile_deplete(self, tile):
        # TODO
        pass
        #self.gui.gui_tiles.set_tile(tile.x, tile.y, tile.get_type_id())

    def _on_tile_change(self, tile):
        for fn in self._listeners["on_tile_change"]:
            fn(tile)

    def add_listener(self, name, fn):
        if name not in self._listeners:
            self._listeners[name] = []

        self._listeners[name].append(fn)




