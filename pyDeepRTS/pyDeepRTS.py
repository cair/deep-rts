import DeepRTS
import pygame
from .gui import GUI


class pyDeepRTS(DeepRTS.Game):

    def __init__(self):
        super(pyDeepRTS, self).__init__()
        self.gui = GUI(self)
        self._render_every = 0

    def render_every(self, interval):
        self._render_every = interval

    def tick(self):
        super().tick()


    def _render(self):
        if self.get_ticks() % self._render_every == 0:
            self.gui.render()



    def _caption(self):
        pygame.display.set_caption("DeepRTS v2.0 - [FPS=%d UPS=%d MUL=x%d]" %
                                   (
                                       self.get_fps(),
                                       self.get_ups(),
                                       self.get_ups() / self.get_ticks_modifier()
                                   ))








