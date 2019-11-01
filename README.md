<p align="center">
  <img src ="./docs/logo.png" />
</p>

## Description [![Build Status](https://travis-ci.org/cair/deep-rts.svg?branch=master)](https://travis-ci.org/cair/deep-rts) [![Documentation](https://img.shields.io/badge/docs-readme-blue.svg)](https://github.com/cair/DeepRTS/blob/c%2B%2B/docs/README.md) [![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/cair/DeepRTS/c%2B%2B/LICENCE.MIT)
A Real-Time Strategy Simulator for (Deep) Reinforment Learning in Python and C++!


## Dependencies

* Python >= 3.5

# Installation
```
sudo pip3 git+https://github.com/cair/DeepRTS.git
```
# Available maps
```
10x10-2-FFA
15x15-2-FFA
21x21-2-FFA
31x31-2-FFA
31x31-4-FFA
31x31-6-FFA
```

# Minimal Example
```python
import random
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
        print("Episode: %s, FPS: %s, UPS: %s" % (episode, game.get_fps(), game.get_ups()))

        terminal = False
        state = env.reset()
        while not terminal:
            action = random.randint(0, 15)  # TODO AI Goes here
            next_state, reward, terminal, _ = env.step(action, render="human")

            state = next_state
```
# In-Game Footage

### 10x10 - 2 Player - free-for-all
<img src="./docs/10x10-2-FFA.png" width="400" height="400">

### 15x15 - 2 Player - free-for-all
<img src="./docs/15x15-2-FFA.png" width="400" height="400">

### 21x21 - 2 Player - free-for-all
<img src="./docs/21x21-2-FFA.png" width="400" height="400">

### 31x31 - 2 Player - free-for-all
<img src="./docs/31x31-2-FFA.png" width="400" height="400">

### 31x31 - 4 Player - free-for-all
<img src="./docs/31x31-4-FFA.png" width="400" height="400">

### 31x3 - 6 Player - free-for-all
<img src="./docs/31x31-6-FFA.png" width="400" height="400">
