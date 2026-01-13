# DeepRTS Quickstart Guide

This guide will get you from zero to running AI training environments in minutes.

## Prerequisites

- Python 3.8+
- pip
- Git
- C++ compiler (for building from source)
- OpenCV (for visualization)

## Installation

### Option 1: Install from Git (Recommended)

```bash
pip install git+https://github.com/cair/DeepRTS.git
```

### Option 2: Install from Source (For Development)

```bash
# Clone with submodules
git clone --recurse-submodules https://github.com/cair/deep-rts.git
cd deep-rts

# Install in development mode
pip install -e .
```

### Option 3: This Repository

```bash
cd deep-rts-playground
pip install -e .
```

### Install Additional Dependencies

```bash
# For visualization
pip install opencv-python

# For ML training
pip install torch numpy

# For Gym-style environments (optional)
pip install gym
```

## Verify Installation

```python
python -c "from DeepRTS import Engine; print('DeepRTS installed successfully!')"
```

---

## Quick Demo: Watch Random Agents Play

Create a file `demo.py`:

```python
from DeepRTS import Engine
import cv2

# Configure game
config = Engine.Config.defaults()
config.set_gui("Blend2DGui")

# Create game (10x10 map, fast to run)
game = Engine.Game("10x10-2p-ffa-Eblil.json", config)

# Add two players
player0 = game.add_player()
player1 = game.add_player()

# Start game
game.set_max_fps(30)  # Slow enough to watch
game.start()

print("Press 'q' to quit")

while not game.is_terminal():
    # Random actions
    player0.do_action(Engine.Random.get(1, 16))
    player1.do_action(Engine.Random.get(1, 16))

    game.update()

    # Show game window
    image = game.render()
    cv2.imshow("DeepRTS Demo", image)

    # Press 'q' to quit
    if cv2.waitKey(33) & 0xFF == ord('q'):
        break

cv2.destroyAllWindows()
print("Game finished!")
```

Run it:
```bash
python demo.py
```

**What you'll see:** A window showing a 2D RTS game with units moving around, harvesting resources, and fighting.

---

## Training Environment Examples

### Example 1: Gold Collection (Simple)

**Goal:** Train an agent to collect gold efficiently.

Create `train_gold.py`:

```python
from DeepRTS import Engine
import numpy as np
import cv2

# Use the GoldCollectFifteen scenario
game = Engine.scenario.GoldCollectFifteen()
player = game.add_player()

game.set_max_fps(0)  # Unlimited speed for training
game.start()

num_episodes = 100
show_every = 10  # Show visualization every N episodes

for episode in range(num_episodes):
    game.reset()
    total_gold = 0
    steps = 0

    while not game.is_terminal() and steps < 1000:
        # Random action (replace with your agent)
        action = Engine.Random.get(1, 16)
        player.do_action(action)

        game.update()
        steps += 1

        # Track gold collected
        total_gold = player.statistic_gathered_gold

        # Visualize occasionally
        if episode % show_every == 0:
            cv2.imshow("Training", game.render())
            cv2.waitKey(1)

    print(f"Episode {episode}: Collected {total_gold} gold in {steps} steps")

cv2.destroyAllWindows()
```

### Example 2: 1v1 Combat (Advanced)

**Goal:** Train agents to fight each other.

Create `train_combat.py`:

```python
from DeepRTS import Engine
import cv2

# Use GeneralAI 1v1 scenario
game = Engine.scenario.GeneralAIOneVersusOne()
player0 = game.add_player()
player1 = game.add_player()

game.set_max_fps(60)
game.start()

for episode in range(100):
    game.reset()

    while not game.is_terminal():
        # Both players take actions
        player0.do_action(Engine.Random.get(1, 16))
        player1.do_action(Engine.Random.get(1, 16))

        game.update()

        # Show the battle
        cv2.imshow("1v1 Combat", game.render())
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # Check who won
    p0_score = player0.get_score()
    p1_score = player1.get_score()
    print(f"Episode {episode}: P0={p0_score:.1f}, P1={p1_score:.1f}")

cv2.destroyAllWindows()
```

### Example 3: DQN Training with PyTorch

**Goal:** Train a neural network agent.

Create `train_dqn.py`:

```python
import torch
import torch.nn as nn
import torch.optim as optim
import numpy as np
from collections import deque
import random
import cv2

from DeepRTS import Engine

# Simple Q-Network
class QNetwork(nn.Module):
    def __init__(self, state_size, action_size):
        super().__init__()
        self.fc1 = nn.Linear(state_size, 256)
        self.fc2 = nn.Linear(256, 128)
        self.fc3 = nn.Linear(128, action_size)

    def forward(self, x):
        x = torch.relu(self.fc1(x))
        x = torch.relu(self.fc2(x))
        return self.fc3(x)

# Setup game
game = Engine.scenario.GoldCollectFifteen()
player = game.add_player()
game.set_max_fps(0)
game.start()

# Get dimensions
width, height = game.get_width(), game.get_height()
state_size = width * height * 10  # 10 channels in state tensor
action_size = 16

# Initialize agent
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
q_network = QNetwork(state_size, action_size).to(device)
optimizer = optim.Adam(q_network.parameters(), lr=1e-4)
memory = deque(maxlen=10000)

# Hyperparameters
epsilon = 1.0
epsilon_decay = 0.995
epsilon_min = 0.01
gamma = 0.99
batch_size = 64

def get_state():
    return np.array(game.state).flatten().astype(np.float32)

def get_action(state, eps):
    if random.random() < eps:
        return random.randint(0, action_size - 1)
    with torch.no_grad():
        state_t = torch.FloatTensor(state).unsqueeze(0).to(device)
        q_values = q_network(state_t)
        return q_values.argmax().item()

def train_step():
    if len(memory) < batch_size:
        return

    batch = random.sample(memory, batch_size)
    states, actions, rewards, next_states, dones = zip(*batch)

    states = torch.FloatTensor(np.array(states)).to(device)
    actions = torch.LongTensor(actions).unsqueeze(1).to(device)
    rewards = torch.FloatTensor(rewards).unsqueeze(1).to(device)
    next_states = torch.FloatTensor(np.array(next_states)).to(device)
    dones = torch.FloatTensor(dones).unsqueeze(1).to(device)

    current_q = q_network(states).gather(1, actions)
    next_q = q_network(next_states).max(1, keepdim=True)[0]
    target_q = rewards + gamma * next_q * (1 - dones)

    loss = nn.MSELoss()(current_q, target_q.detach())
    optimizer.zero_grad()
    loss.backward()
    optimizer.step()

# Training loop
num_episodes = 500
show_every = 50

for episode in range(num_episodes):
    game.reset()
    state = get_state()
    total_reward = 0
    prev_gold = 0

    for step in range(1000):
        # Select and perform action
        action = get_action(state, epsilon)
        player.do_action(action + 1)  # Actions are 1-indexed
        game.update()

        # Get next state and reward
        next_state = get_state()
        current_gold = player.statistic_gathered_gold
        reward = (current_gold - prev_gold) * 10  # Reward for gold
        prev_gold = current_gold
        done = game.is_terminal()

        # Store experience
        memory.append((state, action, reward, next_state, float(done)))

        # Train
        train_step()

        state = next_state
        total_reward += reward

        # Visualize
        if episode % show_every == 0:
            cv2.imshow("DQN Training", game.render())
            cv2.waitKey(1)

        if done:
            break

    epsilon = max(epsilon_min, epsilon * epsilon_decay)

    if episode % 10 == 0:
        print(f"Episode {episode}: Reward={total_reward:.1f}, Gold={current_gold}, Epsilon={epsilon:.3f}")

# Save trained model
torch.save(q_network.state_dict(), "trained_dqn.pth")
print("Model saved!")

cv2.destroyAllWindows()
```

---

## Understanding the Game Window

When you run `cv2.imshow("...", game.render())`, you'll see:

```
┌─────────────────────────────────────┐
│  Green tiles    = Grass (walkable)  │
│  Brown tiles    = Trees (lumber)    │
│  Yellow tiles   = Gold ore          │
│  Gray tiles     = Stone             │
│  Blue tiles     = Water (blocked)   │
│  Red tiles      = Lava (damage)     │
│                                     │
│  Small units    = Peasants/Soldiers │
│  Large units    = Buildings         │
│  Health bars    = Above units       │
└─────────────────────────────────────┘
```

---

## Headless Training (No Visualization)

For faster training, disable rendering:

```python
# Don't call game.render() or cv2.imshow()
# Just run the game loop

game.set_max_fps(0)  # Unlimited speed

for episode in range(10000):
    game.reset()
    while not game.is_terminal():
        player.do_action(action)
        game.update()
        state = game.state  # Get state tensor directly
```

This can achieve **6,000,000+ steps/second**.

---

## Game State Tensor

The `game.state` is a 3D numpy array with shape `[width][height][10]`:

| Channel | Content |
|---------|---------|
| 0 | Tile type ID |
| 1 | Unit present (0/1) |
| 2 | Unit type |
| 3 | Unit health |
| 4 | Unit owner (player ID) |
| 5 | Resources remaining |
| 6 | Walkable (0/1) |
| 7 | Harvestable (0/1) |
| 8-9 | Reserved |

Access it:
```python
state = np.array(game.state)  # Shape: (width, height, 10)
flat_state = state.flatten()  # For fully-connected networks
```

---

## Action Space

16 discrete actions (use values 1-16 with `do_action()`):

| Action ID | Name | Description |
|-----------|------|-------------|
| 1 | PreviousUnit | Select previous unit |
| 2 | NextUnit | Select next unit |
| 3-10 | Movement | 8 directions (Left, Right, Up, Down, diagonals) |
| 11 | Attack | Attack nearby enemy |
| 12 | Harvest | Gather resources |
| 13 | Build0 | Build Farm |
| 14 | Build1 | Build Barracks |
| 15 | Build2 | Build Town Hall |
| 16 | NoAction | Do nothing |

---

## Available Scenarios

| Scenario | Description | Difficulty |
|----------|-------------|------------|
| `GoldCollectFifteen()` | Collect gold | Easy |
| `GeneralAIOneVersusOne()` | 1v1 combat | Medium |
| `LavaMaze()` | Navigate lava hazards | Hard |

```python
# Use scenarios like this:
game = Engine.scenario.GoldCollectFifteen()
game = Engine.scenario.GeneralAIOneVersusOne()
game = Engine.scenario.LavaMaze()
```

---

## Available Maps

| Map File | Size | Players |
|----------|------|---------|
| `10x10-2p-ffa-Eblil.json` | 10x10 | 2 |
| `15x15-2p-ffa-Cresal.json` | 15x15 | 2 |
| `21x21-2p-ffa-Eflines.json` | 21x21 | 2 |
| `31x31-2p-ffa-Thelor.json` | 31x31 | 2 |
| `31x31-4p-ffa-Thelor.json` | 31x31 | 4 |

```python
game = Engine.Game("31x31-2p-ffa-Thelor.json", config)
```

---

## Configuration Options

```python
config = Engine.Config.defaults()

# Game speed
config.set_tick_modifier(10)        # Simulation speed

# Starting resources
config.set_start_gold(1000)
config.set_start_lumber(500)
config.set_start_stone(0)

# Game mechanics
config.set_instant_building(True)   # No build time
config.set_harvest_forever(True)    # Infinite resources
config.set_auto_attack(True)        # Auto-target enemies

# Units/Buildings
config.set_footman(True)            # Enable footman
config.set_archer(True)             # Enable archer
config.set_barracks(True)           # Enable barracks
config.set_farm(True)               # Enable farm

# Display
config.set_gui("Blend2DGui")        # Enable rendering
config.set_console_caption_enabled(True)  # Print FPS
```

---

## Project Structure for Your Experiments

```
my-deeprts-experiment/
├── train.py              # Main training script
├── agent.py              # Your agent implementation
├── models/
│   └── dqn.pth           # Saved models
├── logs/
│   └── training.log      # Training logs
└── requirements.txt      # Dependencies
```

Example `requirements.txt`:
```
DeepRTS @ git+https://github.com/cair/DeepRTS.git
torch
numpy
opencv-python
```

---

## Troubleshooting

### "No module named 'DeepRTS'"
```bash
pip install git+https://github.com/cair/DeepRTS.git
```

### "cv2.imshow doesn't show window"
Make sure you have:
```python
cv2.waitKey(1)  # This is required to update the window
```

### "Game runs too fast to see"
```python
game.set_max_fps(30)  # Limit to 30 FPS
```

### "Build errors"
Install build dependencies:
```bash
# Ubuntu/Debian
sudo apt install build-essential cmake libopencv-dev

# macOS
brew install cmake opencv
```

---

## Next Steps

1. **Start simple:** Run the demos above to understand the game
2. **Implement an agent:** Replace random actions with your algorithm
3. **Train:** Use scenarios with built-in rewards
4. **Experiment:** Try different maps, configurations, and network architectures
5. **Scale up:** Disable rendering for 1000x faster training

For more details, see the other documentation files in `.ai/`:
- [architecture.md](architecture.md) - System internals
- [api-reference.md](api-reference.md) - Full API
- [scenarios.md](scenarios.md) - Scenario/reward system
- [state-machine.md](state-machine.md) - Unit behaviors
