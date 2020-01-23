
# Run setup.py install in sources/python to build source files.
import os
import random
import time

from DeepRTS.Engine import Random
from DeepRTS.python import scenario

if __name__ == "__main__":
    episodes = 10000000
    random_play = True

    env = scenario.GoldCollectOnePlayerFifteen({})
<<<<<<< HEAD:code/main.py
    env.game.set_max_fps(30)
    env.game.set_max_ups(10000000)
=======
    env.game.set_max_fps(99999999)
    env.game.set_max_ups(99999999)

    env.calculate_optimal_play()
>>>>>>> 53c0d69aed448d7b6781f4c955b1c7d2fb4f7a14:coding/main.py

    for episode in range(episodes):
        print("Episode: %s, FPS: %s, UPS: %s" % (episode, env.game.get_fps(), env.game.get_ups()))

        terminal = False
        state = env.reset()
<<<<<<< HEAD:code/main.py
        while not terminal:
            action = random.randint(0, 15)  # TODO AI Goes here
            next_state, reward, terminal, _ = env.step(action)
=======
        #while not terminal:
        #    next_state, reward, terminal, _ = env.step(Random.action())
>>>>>>> 53c0d69aed448d7b6781f4c955b1c7d2fb4f7a14:coding/main.py

            #state = next_state







