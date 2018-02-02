

if __name__ == "__main__":
    from pyDeepRTS import PyDeepRTS
    g = PyDeepRTS()
    player1 = g.add_player()
    player2 = g.add_player()

    g.set_max_fps(10000000)
    g.set_max_ups(10000000)

    g.render_every(50)
    g.capture_every(50)
    g.view_every(50)

    g.start()

    for i in range(100):
        g.tick()
        g.update()
        g.render()
        g.capture()
        g.caption()
        g.view()
        cap = g.gui.capture(save=False, filename="test.png")
        if g.is_terminal():
            g.reset()


