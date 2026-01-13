# DeepRTS Architecture

## System Overview

DeepRTS uses a **hybrid C++/Python architecture** optimized for both performance and ML framework integration.

```
┌─────────────────────────────────────────────────────────────┐
│                    ML Frameworks                             │
│              (Gym, PyTorch, Ray, TensorFlow)                │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                   Python Wrapper Layer                       │
│    DeepRTS/python/game.py, config.py, gui.py, tournament.py │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                   PyBind11 Bindings                          │
│                      bindings/*.cpp                          │
│    Exposes: Game, Player, Unit, Tile, Config, Scenarios     │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                     C++ Engine Core                          │
│                                                              │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐    │
│  │   Game   │  │  Player  │  │   Unit   │  │   Map    │    │
│  │ (334 LOC)│  │ (506 LOC)│  │ (609 LOC)│  │ Tilemap  │    │
│  └──────────┘  └──────────┘  └──────────┘  └──────────┘    │
│                                                              │
│  ┌──────────────────────────────────────────────────────┐  │
│  │              State Machine System                     │  │
│  │   Idle, Walking, Combat, Harvesting, Building, etc.  │  │
│  └──────────────────────────────────────────────────────┘  │
│                                                              │
│  ┌──────────────────────────────────────────────────────┐  │
│  │              Scenario/Criteria System                 │  │
│  │   GoldCollect, DamageDone, GameEnd, etc.             │  │
│  └──────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
```

## Core Game Loop

The engine follows a **tick/update/render** pattern:

```
┌─────────────────────────────────────────────────────────────┐
│                      Game Loop                               │
│                                                              │
│   tick()  ──►  update()  ──►  render()  ──►  caption()      │
│     │            │              │               │            │
│     ▼            ▼              ▼               ▼            │
│  Update      Process        Generate        Print           │
│  clock       units &        visual          FPS             │
│              state          output          stats           │
└─────────────────────────────────────────────────────────────┘
```

### Timing System

```cpp
// FPS control via nanosecond intervals
_update_interval = 1000000000 / max_fps;

// Update only when enough time has elapsed
if (now >= _update_next) {
    for (unit : units) unit.update();
    _update();
    _update_next += _update_interval;
    ticks++;
}
```

## Component Relationships

```
Game (Central Controller)
 │
 ├── players[]           ──► Player instances
 │    └── unitIndexes[]  ──► References to units
 │
 ├── units[]             ──► All Unit instances
 │    ├── state          ──► Current BaseState
 │    ├── stateList[]    ──► Queued states
 │    └── tile           ──► Current Tile position
 │
 ├── tilemap             ──► Tilemap grid
 │    └── tiles[]        ──► Individual Tile objects
 │
 ├── map                 ──► Map data (loaded from JSON)
 │
 ├── state               ──► 3D xtensor [width][height][10]
 │                           (Observation space for ML)
 │
 └── stateManager        ──► Singleton state instances
```

## State Tensor Structure

The game state is exposed as a 3D tensor for ML observation:

```
game.state[x][y][channel]

Channels (10 total):
  0: Tile type ID
  1: Unit present (0/1)
  2: Unit type (if present)
  3: Unit health
  4: Unit owner (player ID)
  5: Resource amount
  6: Walkable (0/1)
  7: Harvestable (0/1)
  8-9: Reserved
```

## Memory Management

### Unit Storage
- Units stored in pre-allocated vector (`MAX_UNITS = 1000` per player)
- Units referenced by index, not pointer
- `unitsNameMap` provides O(1) lookup by nameID

### Tile Storage
- Tiles stored in linear vector: `index = MAP_WIDTH * y + x`
- O(1) access by coordinates
- Dual-state system (new/depleted) avoids runtime allocation

## PyBind11 Integration

### Trampoline Classes
Enable Python subclassing of C++ classes:

```cpp
class PyGame : public Game {
public:
    using Game::Game;

    void _update() override {
        PYBIND11_OVERRIDE(void, Game, _update);
    }
    // ... other virtual methods
};
```

### Return Value Policies
- `py::return_value_policy::reference` - Returns C++ references without copying
- Used for `addPlayer()`, `getUnit()`, etc.

### NumPy Integration
- `NDArrayConverter` enables zero-copy array sharing
- `game.state` exposed directly as NumPy array

## Event System

Virtual callbacks for game events (override in subclass):

```cpp
// Unit lifecycle
virtual void _onUnitCreate(const Unit &unit);
virtual void _onUnitDestroy(const Unit &unit);
virtual void _onUnitAttack(const Unit &unit);

// Resource events
virtual void _onResourceGather(const Tile &tile, const Unit &unit);
virtual void _onResourceDepleted(const Tile &tile, const Unit &unit);

// Game events
virtual void _onEpisodeStart();
virtual void _onEpisodeEnd();
virtual void _onTileChange(const Tile &tile);
```

## Build System Architecture

```
CMakeLists.txt
     │
     ├── VCPKG dependency resolution
     │    └── vcpkg_dependencies.txt
     │
     ├── PyBind11 module compilation
     │    └── bindings/*.cpp
     │
     ├── C++ source compilation
     │    └── src/*.cpp
     │
     ├── Asset embedding (CMRC)
     │    └── DeepRTS/python/assets/*
     │
     └── Output: libdeeprts.so (Python extension)
```

## Key Design Patterns

1. **State Pattern** - Unit behavior via interchangeable state objects
2. **Strategy Pattern** - Pluggable criteria for scenario evaluation
3. **Observer Pattern** - Event callbacks for game state changes
4. **Singleton Pattern** - StateManager, ResourceLoader
5. **Composition** - Scenarios composed of multiple criteria
6. **Template Method** - Virtual `_update()`, `_render()` for extension

## Performance Optimizations

1. **Pre-allocation** - Units and tiles allocated upfront
2. **Index-based references** - Avoids pointer indirection
3. **SIMD via xtensor** - Vectorized state operations
4. **Embedded assets** - No file I/O at runtime
5. **Configurable FPS** - `setMaxFPS(0)` for unlimited speed
6. **Optional rendering** - Skip GUI for headless training
