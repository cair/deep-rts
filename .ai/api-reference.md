# DeepRTS API Reference

## Game Class

**Files:** `include/DeepRTS/Game.h`, `src/Game.cpp`

### Constructors

```cpp
Game(const std::string &map_file);
Game(const std::string &map_file, Config config);
```

### Properties

| Property | Type | Description |
|----------|------|-------------|
| `config` | `const Config` | Game configuration (immutable) |
| `state` | `StateType` | 3D xtensor array [width][height][10] |
| `units` | `std::vector<Unit>` | All units in game |
| `players` | `std::vector<Player>` | Active players |
| `tilemap` | `Tilemap` | Tile grid |
| `map` | `Map` | Map data |
| `ticks` | `uint64_t` | Episode tick counter |
| `episode` | `uint32_t` | Episode number |
| `terminal` | `bool` | Terminal state flag |

### Core Methods

```cpp
// Game loop
void tick();                    // Update internal clock
void update();                  // Process game logic
const cv::Mat& render();        // Generate visual frame
void caption();                 // Print FPS statistics

// Game control
void start();                   // Enable running, init clock
void stop();                    // Disable running
void reset();                   // Reset game state, increment episode
bool isTerminal();              // Check terminal condition

// Player management
Player& addPlayer();            // Add new player (returns reference)
void setSelectedPlayer(Player &player);
```

### Getters

```cpp
Unit& getUnit(int idx);
Unit* getUnitByNameID(const std::string &nameID);
uint32_t getMaxFPS() const;
uint32_t getFPS() const;
uint64_t getGameDuration() const;    // ticks / tickModifier
uint64_t getTicks() const;
uint32_t getWidth() const;
uint32_t getHeight() const;
uint32_t getEpisode() const;
uint32_t getId() const;
long getTicksModifier() const;
```

### Event Callbacks (Virtual)

```cpp
virtual void _onUnitCreate(const Unit &unit);
virtual void _onUnitDestroy(const Unit &unit);
virtual void _onUnitAttack(const Unit &unit);
virtual void _onResourceGather(const Tile &tile, const Unit &unit);
virtual void _onResourceDepleted(const Tile &tile, const Unit &unit);
virtual void _onEpisodeStart();
virtual void _onEpisodeEnd();
virtual void _onTileChange(const Tile &tile);
```

---

## Player Class

**Files:** `include/DeepRTS/Player.h`, `src/Player.cpp`

### Constructor

```cpp
Player(Game &game, int id);
```

### Properties

| Property | Type | Description |
|----------|------|-------------|
| `id_` | `int` | Player ID |
| `name` | `std::string` | Player name |
| `gold` | `int` | Current gold |
| `lumber` | `int` | Current lumber |
| `stone` | `int` | Current stone |
| `food` | `int` | Food capacity |
| `foodConsumption` | `int` | Current food usage |

### Statistics (Per Episode)

| Property | Type | Description |
|----------|------|-------------|
| `sGatheredGold` | `int` | Total gold gathered |
| `sGatheredLumber` | `int` | Total lumber gathered |
| `sGatheredStone` | `int` | Total stone gathered |
| `sDamageDone` | `int` | Damage dealt |
| `sDamageTaken` | `int` | Damage received |
| `sUnitsCreated` | `int` | Units built |

### Unit Counts

```cpp
int num_peasant, num_footman, num_archer;
int num_town_hall, num_farm, num_barrack;
```

### Methods

```cpp
// Resource management
int getGold() const;
int getLumber() const;
int getStone() const;
int getFood() const;
int getFoodConsumption() const;
void addGold(int n);
void addLumber(int n);
void addStone(int n);
void removeGold(int n);
void removeLumber(int n);
void removeStone(int n);

// Unit management
Unit& addUnit(Constants::Unit unitType);
Unit& spawnUnit(Unit &unit, Tile &tile, int select = 1);
void removeUnit(Unit &unit);
Unit& spawn(Tile &spawnPoint);
void spawnUnitNearSpawnPoint(Constants::Unit unitType);
int getUnitCount() const;
Unit* getTargetedUnit();
int getTargetedUnitID() const;
void setTargetedUnitID(int targetedUnitID);

// State
Constants::PlayerState getState() const;
void setState(Constants::PlayerState state);
Constants::PlayerState evaluatePlayerState() const;  // Virtual
double getScore() const;  // Virtual

// Actions
void do_action(int actionID);              // Discrete action (0-15)
void do_manual_action(int id, int x, int y);  // Manual with coordinates
void leftClick(int x, int y);              // Select unit at tile
void rightClick(int x, int y);             // Command unit to tile

// Utility
bool canPlace(Unit &builder, Unit &unit, Tile &tile);
bool canAfford(Unit &unit) const;
void reset();
```

---

## Unit Class

**Files:** `include/DeepRTS/Unit.h`, `src/Unit.cpp`

### Properties

| Property | Type | Description |
|----------|------|-------------|
| `id` | `int` | Unit ID |
| `nameID` | `std::string` | Name ID (e.g., "Peasant1") |
| `typeId` | `Constants::Unit` | Unit type enum |
| `health` | `int` | Current health |
| `health_max` | `int` | Maximum health |
| `direction` | `int` | Facing direction |

### Combat Stats

```cpp
int damageMin, damageMax;
int damageRange;
int damagePiercing;
int armor;
int speed;
int sight;
```

### Capabilities

```cpp
bool canHarvest;
bool canAttack;
bool canMove;
bool military;
bool structure;
bool recallable;
```

### Costs

```cpp
int lumberCost;
int goldCost;
int stoneCost;
int foodProduction;
int foodConsumption;
```

### Carrying Resources

```cpp
float lumberCarry;
float goldCarry;
float stoneCarry;
float carryCapacity;  // Default: 10
```

### State Variables

```cpp
// Current state
std::shared_ptr<BaseState> state;
std::vector<Constants::State> stateList;  // Queued states

// Walking
int walking_timer;
double walking_interval;
std::vector<Tile*> walking_path;
int walkingGoalID;

// Harvesting
int harvestTimer;
double harvestInterval;
int harvestTargetID;

// Combat
int combatTargetID;
double combatTimer;
double combatInterval;

// Building
int buildEntityID;
int buildTimer;

// Spawning
int spawnDuration;
int spawnTimer;
int spawnTileID;
```

### Key Methods

```cpp
// State management
void transitionState();                              // To next queued or Idle
void transitionState(std::shared_ptr<BaseState> nextState);  // Direct
void enqueueState(const std::shared_ptr<BaseState>& state);
void update();  // Called each tick

// Actions
void move(Tile &tile);
void attack(Tile &tile);
void harvest(Tile &tile);
void build(int idx);
void rightClick(Tile &tile);  // Context-sensitive action
void spawn(Tile &spawnPoint);
void despawn();

// Combat
void afflictDamage(int dmg);
int getDamage(Unit &target);

// Utility
Player& getPlayer();
Tile* getNextTile();
```

---

## Config Class

**Files:** `include/DeepRTS/Config.h`

### Setters

```cpp
Config& setTickModifier(int value);          // Game speed (default: 10)
Config& setInstantTownHall(bool value);      // Auto-build town hall
Config& setInstantBuilding(bool value);      // Instant construction
Config& setHarvestForever(bool value);       // Infinite resources
Config& setAutoAttack(bool value);           // Auto-target enemies
Config& setFoodLimit(int value);             // Max food (default: 100)
Config& setFarm(bool value);                 // Enable farm building
Config& setBarracks(bool value);             // Enable barracks
Config& setFootman(bool value);              // Enable footman unit
Config& setArcher(bool value);               // Enable archer unit
Config& setStartGold(int value);             // Initial gold
Config& setStartLumber(int value);           // Initial lumber
Config& setStartStone(int value);            // Initial stone
Config& setStartFood(int value);             // Initial food
Config& setTerminalSignal(bool value);       // End on defeat
Config& setPOMDP(bool value);                // Partial observability
Config& setGUI(const std::string &value);    // "Blend2DGui" or ""
Config& setConsoleCaptionEnabled(bool value);
Config& setTileDamageModifier(double value); // Lava damage multiplier
```

### Properties

| Property | Default | Description |
|----------|---------|-------------|
| `tickModifier` | 10 | Divides tick count |
| `instantTownHall` | true | Auto-build on spawn |
| `instantBuilding` | true | No build time |
| `harvestForever` | false | Infinite harvesting |
| `autoAttack` | true | Auto-target enemies |
| `foodLimit` | 100 | Maximum food |
| `terminalSignal` | true | End when player defeated |
| `startGold/Lumber/Stone` | 0 | Starting resources |
| `startFood` | 1 | Starting food |

### Static Factory

```cpp
static Config defaults();
// Returns: tickModifier=10, instantTownHall=false, instantBuilding=true,
// harvestForever=true, autoAttack=true, foodLimit=100, startGold=1500,
// startLumber=750, terminalSignal=true, gui="Blend2DGui"
```

---

## Tile Class

**Files:** `include/DeepRTS/Tile.h`, `src/Tile.cpp`

### Properties

| Property | Type | Description |
|----------|------|-------------|
| `id` | `int` | Tile ID |
| `x`, `y` | `int` | Coordinates |
| `typeId` | `int` | Current tile type |
| `name` | `std::string` | Tile type name |
| `walkable` | `bool` | Can units walk |
| `harvestable` | `bool` | Can be harvested |
| `resources` | `int` | Resources remaining |
| `depleted` | `bool` | Depleted flag |
| `walkModifier` | `double` | Speed modifier |
| `damageModifier` | `double` | Damage per tick |
| `occupantID` | `int` | Unit on tile (-1 if none) |

### Yield Properties (const)

```cpp
double lumberYield;
double goldYield;
double stoneYield;
```

### Methods

```cpp
// Queries
bool isWalkable() const;      // No occupant AND walkable
bool isHarvestable() const;
bool isBuildable() const;     // Alias for isWalkable
bool isDepleted() const;
bool hasOccupant() const;
int getResources() const;
int distance(Tile &target) const;

// Resource operations
void takeResource(uint8_t n);
void setResources(uint16_t count);
void setDepleted();
void reset();

// Occupant management
void setOccupant(Unit *unit);
Unit* getOccupant() const;

// Getters
double getWalkModifier() const;
double getDamageModifier() const;
double getLumberYield() const;
double getGoldYield() const;
double getStoneYield() const;
```

---

## Tilemap Class

**Files:** `include/DeepRTS/Tilemap.h`, `src/Tilemap.cpp`

### Methods

```cpp
// Tile access
Tile& getTile(uint32_t x, uint32_t y);
std::vector<Tile*> getTileArea(Tile &source, uint32_t width, uint32_t height);
Tile& getSpawnTile(int playerID);

// Neighbor queries
std::vector<Tile*> neighbors(Tile &tile, Constants::Pathfinding type);
// type: Walkable, All, Attackable, Harvestable

// Pathfinding support
bool operator()(unsigned x, unsigned y) const;  // Returns isWalkable

// Reset
void reset();
```

---

## Constants

**File:** `include/DeepRTS/Constants.h`

### Action Enum (16 actions)

```cpp
enum Action {
    PreviousUnit = 1,
    NextUnit = 2,
    MoveLeft = 3,
    MoveRight = 4,
    MoveUp = 5,
    MoveDown = 6,
    MoveUpLeft = 7,
    MoveUpRight = 8,
    MoveDownLeft = 9,
    MoveDownRight = 10,
    Attack = 11,
    Harvest = 12,
    Build0 = 13,    // Farm
    Build1 = 14,    // Barracks
    Build2 = 15,    // Town Hall
    NoAction = 16
};
```

### Unit Types

```cpp
enum Unit {
    Peasant = 1,
    Peon = 2,
    TownHall = 3,
    Barracks = 4,
    Footman = 5,
    Farm = 6,
    Archer = 7,
    None = -1
};
```

### State Enum

```cpp
enum State {
    Spawning = 1,
    Walking = 2,
    Despawned = 3,
    Harvesting = 4,
    Building = 5,
    Combat = 6,
    Dead = 7,
    Idle = 8,
    Base = -1
};
```

### Tile Types

```cpp
const std::unordered_map<std::string, int> TypeToID = {
    {"Spawn",  1},
    {"Grass",  2},
    {"Wall",   3},
    {"Desert", 4},
    {"Water",  5},
    {"Lumber", 6},
    {"Gold",   7},
    {"Stone",  8},
    {"Lava",   9}
};
```

### Limits

```cpp
const int MAX_PLAYERS = 8;
const int MAX_UNITS = 1000;
const int ACTION_MIN = 1;
const int ACTION_MAX = 16;
```

---

## Python Bindings

### Module: `DeepRTS.Engine`

All C++ classes available under `DeepRTS.Engine`:

```python
from DeepRTS import Engine

game = Engine.Game("map.json")
config = Engine.Config()
player = game.add_player()
```

### Python-Specific Classes

```python
from DeepRTS.python import Game, Config, GUI
from DeepRTS.python import scenario

# Extended Python game wrapper
game = Game(map_name, config)

# Scenario environments
env = scenario.GoldCollectFifteen()
env = scenario.GeneralAI_1v1(Config.Map.THIRTYONE)
```
