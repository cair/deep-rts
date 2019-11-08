
# Run setup.py install in sources/python to build source files.
import os
import random
import time

import DeepRTS
from DeepRTS import python
from DeepRTS import Engine

from DeepRTS.python import scenario

if __name__ == "__main__":
    episodes = 10000000
    random_play = True

    env = scenario.GoldCollectOnePlayerFifteen({})
    env.game.set_max_fps(99999999)
    env.game.set_max_ups(99999999)

    env.calculate_optimal_play()

    for episode in range(episodes):
        print("Episode: %s, FPS: %s, UPS: %s" % (episode, env.game.get_fps(), env.game.get_ups()))

        terminal = False
        #state = env.reset()
        while not terminal:
            next_state, reward, terminal, _ = env.step(1)

            #state = next_state







