import DeepRTS
from .gui import GUI


class pyDeepRTS(DeepRTS.Game):

    def __init__(self):
        super(pyDeepRTS, self).__init__()
        self.gui = GUI(self)

    def _render(self):
        self.gui.render()







