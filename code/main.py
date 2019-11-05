
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
    env.game.set_max_fps(10000000)
    env.game.set_max_ups(10000000)

    for episode in range(episodes):
        print("Episode: %s, FPS: %s, UPS: %s" % (episode, env.game.get_fps(), env.game.get_ups()))

        terminal = False
        state = env.reset()
        while not terminal:
            #action = random.randint(0, 15)  # TODO AI Goes here
            next_state, reward, terminal, _ = env.step(0)

            state = next_state







