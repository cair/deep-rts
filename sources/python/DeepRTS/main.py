import pyximport
pyximport.install(language_level=3)
from deeprts import PyDeepRTS
import time

if __name__ == "__main__":
    g = PyDeepRTS('15x15-2v2.json', n_players=2)
    g.set_max_fps(10000000)
    g.set_max_ups(10000000)

    g.render_every(1)
    g.capture_every(1)
    g.view_every(1)

    g.start()

    end = time.time() + 50000
    while time.time() < end:
        g.tick()
        g.update()
        g.render()
        g.caption()
        g.view()


        if g.is_terminal():
            g.reset()

    print("Done!")
