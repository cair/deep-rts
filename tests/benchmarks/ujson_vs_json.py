import json

import time
import ujson
from game.WarC2 import Game


g = Game()
N = 5000
dur1 = 0
dur2 = 0

for y in range(N):

    for x in range(N):
        s1 = time.time()
        ujson.loads(ujson.dumps(g.toJSON()))
        dur1 += time.time() - s1

        s2 = time.time()
        json.loads(json.dumps(g.toJSON()))
        dur2 += time.time() - s2

    for x in range(N):

        s2 = time.time()
        json.loads(json.dumps(g.toJSON()))
        dur2 += time.time() - s2

        s1 = time.time()
        ujson.loads(ujson.dumps(g.toJSON()))
        dur1 += time.time() - s1


    print(dur1, dur2, dur1 - dur2, "ujson", "json")