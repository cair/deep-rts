
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
    gui_config = python.Config(
        render=True,
        view=True,
        inputs=True,
        caption=False,
        unit_health=True,
        unit_outline=False,
        unit_animation=True,
        audio=True,
        audio_volume=50
    )

    engine_config: Engine.Config = Engine.Config.defaults()
    engine_config.set_barracks(True)
    engine_config.set_footman(True)
    engine_config.set_instant_town_hall(True)
    engine_config.set_archer(True)
    engine_config.set_start_wood(2000)
    engine_config.set_start_gold(2000)
    engine_config.set_start_oil(2000)

    game = python.Game(
        python.Config.Map.FIFTEEN,
        n_players=1,  # TODO - Only 1 for some scenarios?
        engine_config=engine_config,
        gui_config=gui_config,
        terminal_signal=False
    )
    game.set_max_fps(30)
    game.set_max_ups(10000000)

    env = scenario.GoldThousand(game)


    for episode in range(episodes):
        print(f"Episode: {episode}, FPS: {game.get_fps()}, UPS: {game.get_ups()}")

        terminal = False
        state = env.reset()
        while not terminal:
            action = random.randint(0, 15)  # TODO AI Goes here
            next_state, reward, terminal, _ = env.step(action, render="human")

            state = next_state






