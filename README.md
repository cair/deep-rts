<img src ="./docs/logo.png" width="50%" height="50%"/>


## Description [![Build Status](https://travis-ci.org/cair/deep-rts.svg?branch=master)](https://travis-ci.org/cair/deep-rts) [![Documentation](https://img.shields.io/badge/docs-readme-blue.svg)](https://github.com/cair/DeepRTS/blob/c%2B%2B/docs/README.md) [![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/cair/DeepRTS/c%2B%2B/LICENCE.MIT)
A Real-Time Strategy Simulator for (Deep) Reinforment Learning in Python and C++!
It is recommended to use the master-branch for the newest (and usually best) version of the environment. I am greatful for any input in regards to improving the environment.

Please use the following citation when using this in your work!
```
@INPROCEEDINGS{8490409,
author={P. {Andersen} and M. {Goodwin} and O. {Granmo}},
booktitle={2018 IEEE Conference on Computational Intelligence and Games (CIG)},
title={Deep RTS: A Game Environment for Deep Reinforcement Learning in Real-Time Strategy Games},
year={2018},
volume={},
number={},
pages={1-8},
keywords={computer games;convolution;feedforward neural nets;learning (artificial intelligence);multi-agent systems;high-performance RTS game;artificial intelligence research;deep reinforcement learning;real-time strategy games;computer games;RTS AIs;Deep RTS game environment;StarCraft II;Deep Q-Network agent;cutting-edge artificial intelligence algorithms;Games;Learning (artificial intelligence);Machine learning;Planning;Ground penetrating radar;Geophysical measurement techniques;real-time strategy game;deep reinforcement learning;deep q-learning},
doi={10.1109/CIG.2018.8490409},
ISSN={2325-4270},
month={Aug},}
```

## Dependencies

* Python >= 3.9.1

# Installation

### Method 1 (From Git Repo)
```
sudo pip3 install git+https://github.com/cair/DeepRTS.git
```

### Method 2 (Clone & Build)
```
git clone https://github.com/cair/deep-rts.git
cd deep-rts
git submodule sync
git submodule update --init
sudo pip3 install .
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
from DeepRTS.python import Config
from DeepRTS.python import scenario

if __name__ == "__main__":
    random_play = True
    episodes = 100

    for i in range(episodes):
        env = scenario.GeneralAI_1v1(Config.Map.THIRTYONE)
        state = env.reset()
        done = False

        while not done:
            env.game.set_player(env.game.players[0])
            action = random.randrange(15)
            next_state, reward, done, _ = env.step(action)
            state = next_state

            if (done):
                break

            env.game.set_player(env.game.players[1])
            action = random.randrange(15)
            next_state, reward, done, _ = env.step(action)
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
