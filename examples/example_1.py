
import numpy

from pyDeepRTS import PyDeepRTS
if __name__ == "__main__":

    g = PyDeepRTS("15x15-2v2.json", pomdp=False, simple=True)
    player1 = g.add_player()
    player2 = g.add_player()

    g.set_agent_player(player1)

    g.set_max_fps(60)
    g.set_max_ups(10)

    #g.render_every(1)
    #g.capture_every(50)
    #g.view_every(1)

    g.start()

    while True:
        g.tick()
        g.update()
        g.render()
        #g.capture()
        g.caption()

        g.view()

        # Capture current state
        #cap = g.gui.capture(save=False, filename="test.png")

        if g.is_terminal():
            g.reset()


        player1.do_action(numpy.random.randint(0, 16))
        #player2.do_action(numpy.random.randint(0, 16))


