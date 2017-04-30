from game import Config
from game.WarC2 import Game
import time

def memory():
    import os
    from wmi import WMI
    w = WMI('.')
    result = w.query("SELECT WorkingSet FROM Win32_PerfRawData_PerfProc_Process WHERE IDProcess=%d" % os.getpid())
    return int(result[0].WorkingSet)

g = Game()

N = 50
dur1 = 0
dur2 = 0
states = []
games = []

for x in range(N):
    s1 = time.time()
    state = g.save()
    dur1 += time.time() - s1
    s2 = time.time()
    gx = g.load(False, state)
    dur2 += time.time() - s2

    states.append(state)
    games.append(gx)

data = {
    'd1': dur1,
    'd2': dur2,
    'tot': dur1 + dur2,
    'avg': (dur1 + dur2) / N,
    'mem': memory()
}

with open("./state_clone_vs_json.txt", "r+") as f:
    try:
        r = f.read()

        existing = Config.LIBRARY_JSON.loads(r)
    except:
        existing = []

    if len(existing) > 10:
        existing.pop(0)

    # Print stats
    for i, it in enumerate(existing):
        print("d1:%s, d2:%s, tot:%s, avg:%s, mem:%s"
              % (
                  data['d1'] - it['d1'],
                  data['d2'] - it['d2'],
                  data['tot'] - it['tot'],
                  data['avg'] - it['avg'],
                  data['mem'] - it['mem']))

    existing.append(data)
    f.seek(0)
    f.write(Config.LIBRARY_JSON.dumps(existing))
    f.truncate()

print("--------")
print("d1:%s, d2:%s, tot:%s, avg:%s, mem:%s"
      % (
          data['d1'],
          data['d2'],
          data['tot'],
          data['avg'],
          data['mem'] / 1024 / 1024))


#
