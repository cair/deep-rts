import DeepRTS
from gui import GUI


class pyDeepRTS(DeepRTS.Game):

    def __init__(self):
        super().__init__()
        self.gui = GUI(self)

    def render(self):
        print(":D")





if __name__ == "__main__":

    g = pyDeepRTS()
    player1 = g.add_player()
    player2 = g.add_player()

    while True:
        g.tick()
        g.update()
        g.render()
        g.get_state()



