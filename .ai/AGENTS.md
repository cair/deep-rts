# DeepRTS - AI Agent Context

> A high-performance Real-Time Strategy (RTS) game engine designed for reinforcement learning research.
> Published at IEEE Conference on Computational Intelligence and Games (CIG) 2018.

## Project Overview

DeepRTS is a fast, affordable alternative to StarCraft II for AI research, providing:
- **6,000,000+ steps/second** without rendering
- **2,000,000+ steps/second** with graphics
- **15,000% faster** than StarCraft II simulation

## Architecture

### Hybrid C++/Python Design

```
C++ Engine (Performance)
        ↓
   PyBind11 Bindings
        ↓
  Python Wrapper Layer
        ↓
ML Frameworks (Gym, PyTorch, Ray, etc.)
```

### Directory Structure

```
deep-rts-playground/
├── include/DeepRTS/           # C++ headers
│   ├── Game.h                 # Main game controller
│   ├── Player.h               # Player management
│   ├── Unit.h                 # Game entities
│   ├── Map.h, Tile.h, Tilemap.h  # World representation
│   ├── Constants.h            # Enums and constants
│   ├── state/                 # Unit state machine classes
│   ├── scenario/              # Scenarios and reward criteria
│   ├── gui/                   # Rendering interfaces
│   ├── unit/                  # Unit type definitions
│   └── util/                  # Pathfinding, math utilities
│
├── src/                       # C++ implementations
│   ├── Game.cpp               # Core game loop (334 lines)
│   ├── Player.cpp             # Player actions (506 lines)
│   ├── Unit.cpp               # Unit behavior (609 lines)
│   ├── state/                 # State implementations
│   ├── scenario/              # Scenario implementations
│   └── gui/                   # GUI implementations
│
├── bindings/                  # PyBind11 C++/Python bindings
│
├── DeepRTS/python/            # Python wrapper and ML integration
│   ├── game.py                # Game wrapper class
│   ├── config.py              # Configuration
│   ├── gui.py                 # GUI rendering (NumPy arrays)
│   ├── tournament.py          # Multi-agent tournaments
│   ├── ml/                    # ML utilities
│   └── assets/                # Game resources
│       ├── maps/              # JSON map definitions
│       ├── textures/          # PNG sprites
│       └── tile_properties.json
│
├── examples/                  # Example implementations
│   └── example_05/            # PyTorch agents (CNN/FC)
│
├── CMakeLists.txt             # Build configuration
├── setup.py                   # Python package setup
└── pyproject.toml             # Python build config
```

## Core Components

### Game Engine (`Game.h`, `Game.cpp`)

Central controller with main loop:
- `tick()` - Advances game simulation
- `update()` - Processes game state
- `render()` - Generates visual output

### Players (`Player.h`, `Player.cpp`)

Manages:
- Unit ownership and control
- Resources: Gold, Lumber, Stone, Food
- Actions and commands

### Units (`Unit.h`, `Unit.cpp`)

Game entities with:
- Type-specific properties (Peasant, Archer, Footman, buildings)
- State machine for behavior
- Combat, harvesting, building capabilities

### State Machine (`include/DeepRTS/state/`)

Unit states follow this pattern:
```
Spawning → Idle → {Walking, Harvesting, Building, Combat} → Dead
```

States:
- `BaseState` - Abstract base with `init()`, `update()`, `end()`
- `Idle` - Default waiting state
- `Walking` - Movement with pathfinding
- `Harvesting` - Resource gathering
- `Combat` - Attacking enemies
- `Building` - Constructing structures
- `Spawning` - Unit creation
- `Dead` - Unit destroyed

### Scenarios (`include/DeepRTS/scenario/`)

Pre-built training environments:
- `GoldCollectFifteen` - Collect 1000 gold
- `GeneralAIOneVersusOne` - 1v1 combat
- `LavaMaze` - Navigation challenge

**Criteria System** - Modular rewards:
- `GoldCollect`, `DamageDone`, `DamageTaken`
- `UnitsCreated`, `FoodConsumption`, `GameEnd`

## Action Space

16 discrete actions (defined in `Constants.h`):

| ID | Action | Description |
|----|--------|-------------|
| 0 | PreviousUnit | Select previous unit |
| 1 | NextUnit | Select next unit |
| 2-9 | Movement | 8 directional movements |
| 10 | Attack | Attack enemy |
| 11 | Harvest | Gather resources |
| 12-14 | Build | Construct buildings |
| 15 | NoAction | Do nothing |

## Resource System

Four resource types:
- **Gold** - From ore mining
- **Lumber** - From tree harvesting
- **Stone** - From quarry mining
- **Food** - Limits unit production

## Unit Types

| Type | Role | Capabilities |
|------|------|--------------|
| Peasant | Worker | Harvest, Build |
| Archer | Military | Ranged attack |
| Footman | Military | Melee attack |
| Town Hall | Building | Production, storage |
| Barracks | Building | Produces soldiers |
| Farm | Building | Food production |

## Build System

### Dependencies (via VCPKG)
- PyBind11 - C++/Python bindings
- xtensor + XSIMD - Numeric arrays with SIMD
- OpenCV - Image processing
- Blend2D - 2D graphics
- spdlog - Logging
- nlohmann_json - JSON parsing

### Building

```bash
# Install from git
pip3 install git+https://github.com/cair/DeepRTS.git

# Or from source
git clone --recurse-submodules https://github.com/cair/deep-rts.git
pip3 install .
```

## Usage Examples

### Python (Recommended)

```python
from DeepRTS.python import Config, scenario

# Create environment
env = scenario.GeneralAI_1v1(Config.Map.THIRTYONE)
state = env.reset()

# Game loop
done = False
while not done:
    action = random.randrange(15)
    next_state, reward, done, info = env.step(action)
```

### C++

```cpp
auto g = DeepRTS::Scenarios::LavaMaze();
Player &player0 = g.addPlayer();
Player &player1 = g.addPlayer();
g.setMaxFPS(0);
g.start();

while (!g.isTerminal()) {
    player0.do_action(randomAction);
    player1.do_action(randomAction);
    g.update();
    cv::imshow("Game", g.render());
}
```

## Key Files Reference

| File | Lines | Purpose |
|------|-------|---------|
| `src/Game.cpp` | 334 | Core game loop |
| `src/Unit.cpp` | 609 | Unit behavior (most complex) |
| `src/Player.cpp` | 506 | Player actions |
| `include/DeepRTS/Game.h` | 229 | Game interface |
| `include/DeepRTS/Constants.h` | - | All enums and constants |
| `DeepRTS/python/game.py` | - | Python game wrapper |
| `DeepRTS/python/gui.py` | ~26KB | Python rendering |

## Development Notes

- C++17 standard required
- Assets are embedded into binary via CMRC (no external files at runtime)
- Supports 2-8 players on various map sizes (10x10 to 31x31)
- JPS (Jump Point Search) for pathfinding

---

## Additional Documentation

For deeper context, see the following files in this directory:

| File | Description |
|------|-------------|
| [quickstart.md](quickstart.md) | **Start here!** Installation, demos, training examples |
| [architecture.md](architecture.md) | System design, game loop, component relationships |
| [api-reference.md](api-reference.md) | Complete API for Game, Player, Unit, Tile, Config |
| [scenarios.md](scenarios.md) | Training scenarios and criteria system |
| [state-machine.md](state-machine.md) | Unit behavior states and transitions |
| [maps-and-tiles.md](maps-and-tiles.md) | World system, tile types, resource mechanics |
| [building.md](building.md) | Build system, dependencies, troubleshooting |
