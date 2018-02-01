
import numpy

from pyDeepRTS.pyDeepRTS import pyDeepRTS


if __name__ == "__main__":

    g = pyDeepRTS()
    player1 = g.add_player()
    player2 = g.add_player()

    g.set_max_fps(100000000)
    g.set_max_ups(100000000)


    g.render_every(50)
    g.capture_every(50)
    g.view_every(50)


    g.start()

    while True:
        g.tick()
        g.update()
        g.render()
        g.capture()
        g.caption()

        g.view()


        cap = g.gui.capture(save=False, filename="test.png")



        if g.is_terminal():
            g.reset()

        player1.queue_action(numpy.random.randint(0, 15), 1)



