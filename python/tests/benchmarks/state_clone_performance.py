from game import Config
from game.Game import Game
import time


g = Game(ai_instance=True)
g.init()

N = 1
IT = 100
dur1 = 0
dur2 = 0
states = []
games = []
for y in range(IT):
    for x in range(N):
        s1 = time.time()
        state = g.save()
        dur1 += time.time() - s1
        s2 = time.time()
        g = g.load(False, state, ai_instance=True)
        dur2 += time.time() - s2
        state['players'][0]['units'].append("FUCK MY LIFE")

    print("Save: %s, Load: %s, StateL: %s" % (dur1, dur2, len(Config.LIBRARY_JSON.dumps(state))))
    dur1 = 0
    dur2 = 0

