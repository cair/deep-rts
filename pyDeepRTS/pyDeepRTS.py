import DeepRTS

from pyDeepRTS.pyglet.gui import GUI as PygletGUI
from pyDeepRTS.pygame.gui import GUI as PygameGUI


class pyDeepRTS(DeepRTS.Game):

    def __init__(self):
        super(pyDeepRTS, self).__init__()

        self.gui = PygameGUI(self)
        self._render_every = 1
        self._view_every = 1
        self._capture_every = 1

    def render_every(self, interval):
        self._render_every = interval

    def tick(self):
        super().tick()

    def _render(self):
        if self.get_ticks() % self._render_every == 0:
            self.gui.render()

    def view(self):
        if self.get_ticks() % self._view_every == 0:
            self.gui.view()

    def capture(self):
        if self.get_ticks() % self._capture_every == 0:
            return self.gui.capture()
        return None

    def view_every(self, n):
        self._view_every = n

    def capture_every(self, n):
        self._capture_every = n

    def _caption(self):
        self.gui.set_caption("DeepRTS v2.0 - [FPS=%d UPS=%d MUL=x%d]" %
        (
        self.get_fps(),
        self.get_ups(),
        self.get_ups() / self.get_ticks_modifier()
        ))

        pass

    def _on_unit_create(self, unit):
        pass

    def _on_unit_destroy(self, unit):
        pass

    def _on_episode_start(self):
        for tile in self.map.tiles:
            self.gui.gui_tiles.set_tile(tile.x, tile.y, tile.get_type_id())

    def _on_episode_end(self):
        pass

    def _on_tile_deplete(self, tile):
        self.gui.gui_tiles.set_tile(tile.x, tile.y, tile.get_type_id())





