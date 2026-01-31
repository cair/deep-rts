# Component Architecture

> Detailed component relationships and responsibilities.

## Component Hierarchy

```
DeepRTS
├── Core Engine (C++)
│   ├── Game              # Main controller, game loop
│   ├── Player            # Player management, resources
│   ├── Unit              # Game entities, behavior
│   ├── Tile              # World cell representation
│   ├── Tilemap           # World grid management
│   ├── Map               # Map loading, dimensions
│   ├── Config            # Game configuration
│   └── StateManager      # Unit state machine orchestration
│
├── State Machine (C++)
│   ├── BaseState         # Abstract state interface
│   ├── Idle              # Default waiting state
│   ├── Walking           # Movement with pathfinding
│   ├── Harvesting        # Resource gathering
│   ├── Combat            # Attack behavior
│   ├── Building          # Structure construction
│   ├── Spawning          # Unit creation
│   ├── Dead              # Destruction handling
│   └── Despawned         # Removal from game
│
├── Scenario System (C++)
│   ├── Scenario          # Base scenario class (extends Game)
│   ├── GoldCollectFifteen
│   ├── GeneralAIOneVersusOne
│   ├── LavaMaze
│   └── Criteria System
│       ├── ScenarioCriteria   # Abstract criterion
│       ├── GoldCollect
│       ├── DamageDone/Taken
│       ├── UnitsCreated
│       └── ... (15 criteria types)
│
├── GUI System
│   ├── BaseGUI (C++)     # Abstract rendering interface
│   ├── Blend2DGUI (C++)  # Fast native rendering
│   └── GUI (Python)      # Pygame-based rich UI
│
├── Bindings Layer
│   ├── DeepRTS.cpp       # Module entry point
│   ├── Game.cpp          # Game class bindings
│   ├── Player.cpp        # Player class bindings
│   ├── Unit.cpp          # Unit class bindings
│   ├── Scenarios.cpp     # Scenario bindings
│   └── Trampolines/      # Python inheritance support
│
└── Python Layer
    ├── game.py           # Python Game wrapper
    ├── gui.py            # Pygame GUI implementation
    ├── config.py         # Python config wrapper
    └── tournament.py     # Multi-agent tournaments
```

## Core Component Details

### Game (src/Game.cpp, include/DeepRTS/Game.h)

**Responsibility**: Central controller managing game loop, players, and units.

```cpp
class Game {
    // Core state
    Config config;
    Map map;
    StateType state;           // xt::xarray for observations
    Tilemap tilemap;
    StateManager stateManager;
    std::vector<Player> players;
    std::vector<Unit> units;
    
    // Core methods
    void tick();               // Update game clock
    void update();             // Process game state
    cv::Mat& render();         // Generate visual output
    Player& addPlayer();       // Add player to game
    void reset();              // Reset for new episode
    bool isTerminal();         // Check win/loss conditions
};
```

**Key Relationships**:
- Owns all `Player` and `Unit` instances
- Delegates rendering to `BaseGUI` implementation
- Drives state machine updates via `StateManager`

### Player (src/Player.cpp, include/DeepRTS/Player.h)

**Responsibility**: Manages player resources, units, and actions.

```cpp
class Player {
    // Resources
    int gold, lumber, stone;
    int food, foodConsumption;
    
    // Statistics
    int sGatheredGold, sDamageDone, sUnitsCreated;
    
    // Unit tracking
    std::vector<int> unitIndexes;
    int targetedUnitID;
    
    // Actions
    void do_action(int action);
    void spawn(Tile& spawnPoint);
    Unit& addUnit(Constants::Unit type);
};
```

**Key Relationships**:
- References parent `Game`
- Owns indices to units (actual units stored in Game)
- Executes 16 discrete actions

### Unit (include/DeepRTS/unit/Unit.h, src/Unit.cpp)

**Responsibility**: Individual game entity with type-specific behavior.

```cpp
class Unit {
    // Identity
    int id;
    std::string nameID;
    Constants::Unit typeId;
    
    // Combat stats
    int health, health_max;
    int damageMin, damageMax, armor;
    
    // Resource carrying
    float goldCarry, lumberCarry, stoneCarry;
    
    // State machine
    StateManager* stateManager;
    std::shared_ptr<BaseState> state;
    
    // Behavior
    void update();
    void move(Tile& target);
    void attack(Tile& target);
    void harvest(Tile& target);
    bool build(int idx);
};
```

**Unit Types** (Constants::Unit):
| Type | Role | Capabilities |
|------|------|--------------|
| Peasant (1) | Worker | Harvest, Build |
| Peon (2) | Worker (Orc) | Harvest, Build |
| TownHall (3) | Building | Production hub |
| Barracks (4) | Building | Military production |
| Footman (5) | Military | Melee combat |
| Farm (6) | Building | Food production |
| Archer (7) | Military | Ranged combat |

### Tile (include/DeepRTS/Tile.h, src/Tile.cpp)

**Responsibility**: Single cell in the game world.

```cpp
class Tile {
    // State (original and depleted)
    bool harvestable, walkable;
    int typeId, resources;
    double walkModifier, damageModifier;
    
    // Occupancy
    int occupantID;  // -1 if empty
    
    // Position
    int x, y;
    int id;
};
```

**Tile Types** (Constants::TypeToID):
| Type | ID | Properties |
|------|-----|------------|
| Spawn | 1 | Player starting point |
| Grass | 2 | Walkable terrain |
| Wall | 3 | Impassable |
| Desert | 4 | Walkable |
| Water | 5 | Impassable |
| Lumber | 6 | Harvestable wood |
| Gold | 7 | Harvestable gold |
| Stone | 8 | Harvestable stone |
| Lava | 9 | Damage-dealing |

### StateManager (include/DeepRTS/state/StateManager.h)

**Responsibility**: Orchestrates unit state transitions.

```cpp
class StateManager {
    Game& game;
    
    // State singletons
    std::shared_ptr<Idle> idleState;
    std::shared_ptr<Walking> walkingState;
    // ... etc
    
    std::shared_ptr<BaseState> getState(Constants::State id);
};
```

## Component Interaction Diagram

```
┌──────────┐     owns      ┌──────────┐     owns      ┌──────────┐
│   Game   │──────────────▶│  Player  │──────────────▶│   Unit   │
└──────────┘               └──────────┘               └──────────┘
     │                          │                          │
     │ owns                     │ do_action()              │ has state
     ▼                          │                          ▼
┌──────────┐                    │                    ┌──────────┐
│  Tilemap │◀───────────────────┘                    │BaseState │
└──────────┘     queries                             └──────────┘
     │                                                     │
     │ contains                                            │ transition
     ▼                                                     ▼
┌──────────┐                                         ┌──────────┐
│   Tile   │◀────────────────────────────────────────│StateMgr  │
└──────────┘         occupies/harvests               └──────────┘
```

## Binding Component Details

### Trampoline Classes

Enable Python subclassing of C++ classes:

| Trampoline | Base Class | Purpose |
|------------|------------|---------|
| `PyGame` | `Game` | Override callbacks in Python |
| `PyPlayer` | `Player` | Custom player logic |
| `PyScenarioCriteria` | `ScenarioCriteria` | Custom reward criteria |

```cpp
// Example: PyGame trampoline
class PyGame : public Game {
public:
    using Game::Game;
    
    void _onUnitCreate(const Unit& unit) const override {
        PYBIND11_OVERRIDE(void, Game, _onUnitCreate, unit);
    }
    // ... other callbacks
};
```
