# DeepRTS Unit State Machine

## Overview

Units in DeepRTS use a **state machine pattern** to manage behavior. Each unit has a current state and a queue of pending states.

## State Diagram

```
                    ┌──────────────┐
                    │   Despawned  │  (Initial state)
                    └──────┬───────┘
                           │ unit.spawn()
                           ▼
                    ┌──────────────┐
                    │   Spawning   │
                    └──────┬───────┘
                           │ spawnTimer >= spawnDuration
                           ▼
┌──────────────────────────────────────────────────────────┐
│                         Idle                              │
│                   (Default state)                         │
└──────┬───────────────┬───────────────┬───────────────┬───┘
       │               │               │               │
       │ move()        │ attack()      │ harvest()     │ build()
       ▼               ▼               ▼               ▼
┌──────────────┐ ┌──────────────┐ ┌──────────────┐ ┌──────────────┐
│   Walking    │ │    Combat    │ │  Harvesting  │ │   Building   │
└──────┬───────┘ └──────┬───────┘ └──────┬───────┘ └──────┬───────┘
       │               │               │               │
       │ path empty    │ target dies   │ complete      │ buildTimer done
       │               │               │               │
       └───────────────┴───────────────┴───────────────┘
                           │
                           ▼
                    ┌──────────────┐
                    │     Idle     │  (or queued state)
                    └──────────────┘

                    ┌──────────────┐
                    │     Dead     │  ◄── health <= 0 (from any state)
                    └──────────────┘
```

## State Constants

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

---

## State Classes

### BaseState (Abstract)

**File:** `include/DeepRTS/state/BaseState.h`

```cpp
class BaseState {
public:
    Constants::State id;
    std::string name;

    virtual void init(Unit &unit) = 0;    // Called on state entry
    virtual void update(Unit &unit) = 0;  // Called each tick
    virtual void end(Unit &unit) = 0;     // Called on state exit
};
```

---

### Spawning State

**Purpose:** Handles unit spawn animation/delay

**Behavior:**
1. `init()`: Empty
2. `update()`:
   - Increment `spawnTimer`
   - Wait until `spawnTimer >= spawnDuration`
   - Check tile occupancy, find alternative if needed
   - Set initial direction (Up for structures, Down for units)
   - Transition to next state
3. `end()`: Clear `builtByID`

**Key Variables:**
- `unit.spawnTimer` - Current spawn progress
- `unit.spawnDuration` - Total spawn time
- `unit.spawnTileID` - Target tile

**Transitions:**
- → Idle (or queued state) when spawn complete

---

### Idle State

**Purpose:** Default resting state

**Behavior:**
1. `init()`: Empty
2. `update()`: Empty (no active behavior)
3. `end()`: Empty

**Transitions:**
- → Walking (via `unit.move()`)
- → Combat (via `unit.attack()`)
- → Harvesting (via `unit.harvest()`)
- → Building (via `unit.build()`)

---

### Walking State

**Purpose:** Unit movement with pathfinding

**Behavior:**
1. `init()`:
   - Get target tile via `walkingGoalID`
   - Find first walkable tile if goal is occupied
   - Calculate path using JPS (Jump Point Search)
   - Set initial direction
2. `update()`:
   - Apply walk modifier from current tile
   - Increment `walking_timer`
   - When `walking_timer >= walking_interval * walkModifier`:
     - Pop next tile from path
     - Move unit to tile
     - Check walkability
     - Decrement `stepsLeft` if limited
     - Transition when path empty or `stepsLeft == 1`
3. `end()`: Clear `walking_path`

**Key Variables:**
- `unit.walking_path` - Vector of tiles to traverse
- `unit.walking_timer` - Progress toward next tile
- `unit.walking_interval` - Base movement speed
- `unit.walkingGoalID` - Target tile
- `unit.stepsLeft` - Step limit (0 = unlimited)

**Transitions:**
- → Idle when path complete
- → Idle when blocked
- → (queued state) if state was enqueued

---

### Harvesting State

**Purpose:** Resource gathering (3-phase cycle)

**Behavior:**

**Phase 0 - Travel to Resource:**
- Walk to harvestable tile if distance > 1
- Use pathfinding to find route
- Transition to Walking with Harvesting enqueued

**Phase 1 - Harvest:**
- Wait for `harvestInterval` ticks
- Gather resources based on tile yields:
  - `lumberCarry += tile.lumberYield * config.yieldModifierLumber`
  - `goldCarry += tile.goldYield * config.yieldModifierGold`
  - `stoneCarry += tile.stoneYield * config.yieldModifierStone`
- Update player statistics
- Deplete tile resources
- Check if at carry capacity or tile depleted
- Auto-find next harvestable tile

**Phase 2 - Return to Base:**
- Walk to closest recall building
- Deliver resources to player
- Clear carry amounts
- Loop if `config.harvestForever` enabled

**Key Variables:**
- `unit.harvestIterator` - Current phase (0, 1, or 2)
- `unit.harvestTimer` - Progress within phase
- `unit.harvestInterval` - Ticks per harvest action
- `unit.harvestTargetID` - Current resource tile
- `unit.carryCapacity` - Max resources (default: 10)

**Transitions:**
- → Walking (with Harvesting enqueued) for travel
- → Idle when no resources remain
- → Harvesting (loop) if `harvestForever` enabled

---

### Combat State

**Purpose:** Unit-to-unit combat

**Behavior:**
1. `init()`:
   - Set direction toward target
2. `update()`:
   - Increment `combatTimer`
   - When `combatTimer >= combatInterval`:
     - Validate target (alive, has tile)
     - If out of range:
       - Calculate `stepsLeft = distance - damageRange`
       - Transition to Walking with Combat enqueued
     - If in range:
       - Calculate damage: `getDamage(target)`
       - Apply damage to target
       - Update statistics
       - If target dies, clear combat
       - If target is Idle, provoke into combat
     - Reset `combatTimer`
3. `end()`: Empty

**Damage Formula:**
```cpp
damage = (damageMin + Random(damageMin, damageMax)
          - target.armor * 0.12
          + damagePiercing)
         * Random(0.5, 1.0);
damage = max(0, damage);
```

**Key Variables:**
- `unit.combatTargetID` - Target unit ID
- `unit.combatTimer` - Progress toward next attack
- `unit.combatInterval` - Ticks between attacks
- `unit.damageMin`, `unit.damageMax` - Damage range
- `unit.damageRange` - Maximum attack distance
- `unit.damagePiercing` - Armor-ignoring damage

**Transitions:**
- → Walking (with Combat enqueued) if out of range
- → Idle when target dies/despawns

---

### Building State

**Purpose:** Construction of buildings/units

**Behavior:**
1. `init()`: Empty (build timer reset on `end()`)
2. `update()`:
   - Increment `buildTimer`
   - When `buildTimer >= buildEntity.spawnDuration`:
     - If builder has no tile, find walkable tile
     - Update player food/consumption
     - Transition to next state
3. `end()`:
   - Clear `buildEntityID`
   - Reset `buildTimer`

**Key Variables:**
- `unit.buildEntityID` - Unit being constructed
- `unit.buildTimer` - Construction progress

**Transitions:**
- → Idle when construction complete

---

### Dead State

**Purpose:** Final state on unit death

**Behavior:**
1. `init()`: Empty
2. `update()`:
   - Call `unit.despawn()`
   - Remove unit from player's list
   - Re-evaluate player state
   - Check if game is terminal
3. `end()`: Empty

**Trigger:**
- `unit.afflictDamage()` when `health <= 0`

---

### Despawned State

**Purpose:** Unit not in game world

**Behavior:**
- All methods empty (no behavior)
- Initial state for all units before spawning

**Transitions:**
- → Spawning (via `unit.spawn()`)

---

## State Management

### Unit State Methods

```cpp
// Transition to next queued state or Idle
void Unit::transitionState();

// Direct transition to specific state
void Unit::transitionState(std::shared_ptr<BaseState> nextState);

// Queue state for later execution
void Unit::enqueueState(const std::shared_ptr<BaseState>& state);

// Called each game tick
void Unit::update() {
    state->update(*this);
    if (tile) tile->update(*this);
}
```

### State Queue

Units maintain a `stateList` vector for sequential state execution:

```cpp
std::vector<Constants::State> stateList;
```

When transitioning:
1. Call current state's `end()`
2. Pop next state from queue (or default to Idle)
3. Set new state
4. Call new state's `init()`
5. Reset `animationCounter`

### StateManager

Singleton providing state instance lookup:

```cpp
class StateManager {
    std::shared_ptr<BaseState> getByID(int id);
    // Returns singleton instances of each state
};
```

---

## Transition Table

| From State | Trigger | To State |
|------------|---------|----------|
| Despawned | `spawn()` | Spawning + Idle (enqueued) |
| Spawning | timer complete | Idle (or queued) |
| Idle | `move(tile)` | Walking |
| Idle | `attack(tile)` | Combat |
| Idle | `harvest(tile)` | Harvesting |
| Idle | `build(idx)` | Building + Idle (enqueued) |
| Walking | path empty | Idle (or queued) |
| Walking | blocked | Idle |
| Walking | `stepsLeft == 1` | Idle (or queued) |
| Combat | target dies | Idle |
| Combat | out of range | Walking + Combat (enqueued) |
| Harvesting (0) | distance > 1 | Walking + Harvesting (enqueued) |
| Harvesting (1) | capacity reached | Harvesting (Phase 2) |
| Harvesting (1) | no resources | Idle |
| Harvesting (2) | delivered + forever | Harvesting (loop) |
| Harvesting (2) | delivered + !forever | Idle |
| Building | timer complete | Idle |
| Any | `afflictDamage()` + health <= 0 | Dead |
| Any (except Dead/Despawned) | `despawn()` | Despawned |

---

## Timing Configuration

State timers are scaled by `config.tickModifier`:

```cpp
// In Unit constructor
harvestInterval = 0.5 * config.tickModifier;
combatInterval = 1.0 * config.tickModifier;
walking_interval = 1.0 * config.tickModifier;
```

Default `tickModifier = 10`, so:
- Harvest every 5 ticks
- Attack every 10 ticks
- Move every 10 ticks (modified by terrain)

---

## File Locations

| State | Header | Implementation |
|-------|--------|----------------|
| BaseState | `include/DeepRTS/state/BaseState.h` | `src/state/BaseState.cpp` |
| StateManager | `include/DeepRTS/state/StateManager.h` | `src/state/StateManager.cpp` |
| Idle | `include/DeepRTS/state/Idle.h` | `src/state/Idle.cpp` |
| Walking | `include/DeepRTS/state/Walking.h` | `src/state/Walking.cpp` |
| Combat | `include/DeepRTS/state/Combat.h` | `src/state/Combat.cpp` |
| Harvesting | `include/DeepRTS/state/Harvesting.h` | `src/state/Harvesting.cpp` |
| Building | `include/DeepRTS/state/Building.h` | `src/state/Building.cpp` |
| Spawning | `include/DeepRTS/state/Spawning.h` | `src/state/Spawning.cpp` |
| Dead | `include/DeepRTS/state/Dead.h` | `src/state/Dead.cpp` |
| Despawned | `include/DeepRTS/state/Despawned.h` | `src/state/Despawned.cpp` |
