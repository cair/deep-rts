import random

from pyDeepRTS.pyDeepRTS import pyDeepRTS
import time


if __name__ == "__main__":

    g = pyDeepRTS()
    player1 = g.add_player()
    player2 = g.add_player()


    g.set_max_fps(100000)
    g.set_max_ups(100000)
    g.render_every(10)

    g.start()

    while True:
        g.tick()
        g.update()
        g.render()
        g.get_state()
        g.caption()


        if g.is_terminal():
            g.reset()

        player1.queue_action(random.randint(0, 13), 1)



