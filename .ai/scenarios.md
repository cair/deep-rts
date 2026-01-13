# DeepRTS Scenarios and Criteria System

## Overview

Scenarios are pre-built training environments that extend the base Game class with specific win conditions and reward structures. They use a **composition pattern** where multiple criteria are combined to define success.

## Architecture

```
Scenario (extends Game)
    │
    ├── criteriaList[8]     ──► Per-player criteria sets
    │    └── criteria[]     ──► Individual ScenarioCriteria objects
    │
    └── evaluate(player)    ──► ALL criteria must pass (AND logic)
```

## Available Scenarios

### 1. GoldCollectFifteen

**Purpose:** Simple resource collection tutorial

**File:** `include/DeepRTS/scenario/scenarios/GoldCollectFifteen.h`

| Setting | Value |
|---------|-------|
| Map | EBLIL (10x10) |
| Players | 1 |
| Win Condition | Collect 10 gold |

**Configuration:**
- Default game config
- Single player environment

**Optimal Strategy:**
```
Move Right x4 → Move Down-Right x1 → Move Right x1
```

---

### 2. GeneralAIOneVersusOne

**Purpose:** Complex 1v1 combat scenario testing diverse gameplay

**File:** `include/DeepRTS/scenario/scenarios/GeneralAIOneVersusOne.h`

| Setting | Value |
|---------|-------|
| Map | VASTRANA (21x21) |
| Players | 2 |
| Starting Resources | 1000 each (Lumber, Gold, Stone) |

**Configuration:**
```cpp
config.setFootman(true)
      .setArcher(true)
      .setBarracks(true)
      .setFarm(true)
      .setInstantTownHall(true)
      .setInstantBuilding(true)
      .setStartLumber(1000)
      .setStartGold(1000)
      .setStartStone(1000);
```

**Win Criteria (ALL must be satisfied):**

| Criterion | Target |
|-----------|--------|
| ResourceIncrement(Lumber) | 10,000 |
| ResourceIncrement(Gold) | 10,000 |
| ResourceIncrement(Stone) | 10,000 |
| ResourceIncrement(Food) | 10,000 |
| UnitIncrement(TownHall) | 10,000 |
| UnitIncrement(Barracks) | 10,000 |
| UnitIncrement(Farm) | 10,000 |
| UnitIncrement(Peasant) | 10,000 |
| UnitIncrement(Archer) | 10,000 |
| UnitIncrement(Footman) | 10,000 |
| DamageDoneIncrement | 10,000 |

---

### 3. LavaMaze

**Purpose:** Survival/puzzle scenario with environmental hazards

**File:** `include/DeepRTS/scenario/scenarios/LavaMaze.h`

| Setting | Value |
|---------|-------|
| Map | GONDOR (21x21) |
| Players | 1 |
| Starting Resources | 0 (all) |
| Tile Damage Modifier | 100.0x |

**Configuration:**
```cpp
config.setFootman(true)
      .setArcher(true)
      .setBarracks(true)
      .setFarm(true)
      .setInstantTownHall(true)
      .setInstantBuilding(true)
      .setTileDamageModifier(100.0)  // Very high lava damage
      .setStartLumber(0)
      .setStartGold(0)
      .setStartStone(0);
```

**Win Criteria:**
- ResourceIncrement(Stone) by 1

**Challenge:** Navigate through lava tiles that deal massive damage while collecting minimal resources.

---

## Criteria Classes

All criteria inherit from `ScenarioCriteria`:

```cpp
class ScenarioCriteria {
    const int rewardSuccess;   // Default: 0.01
    const int rewardFailure;   // Default: -0.01

    virtual bool evaluate(const Player& player) = 0;
    virtual int reward() const = 0;
    virtual std::shared_ptr<ScenarioCriteria> clone() const = 0;
};
```

### Resource Collection (Absolute Thresholds)

#### GoldCollect
```cpp
GoldCollect(int goldCollectedLimit, int rewardSuccess=0.01, int rewardFailure=-0.01)
```
- **Evaluates:** `player.sGatheredGold >= goldCollectedLimit`
- **Use:** Track total gold collection

#### LumberCollect
```cpp
LumberCollect(int lumberCollectLimit, int rewardSuccess=0.01, int rewardFailure=-0.01)
```
- **Evaluates:** `player.sGatheredLumber >= lumberCollectLimit`

#### StoneCollect
```cpp
StoneCollect(int stoneCollectLimit, int rewardSuccess=0.01, int rewardFailure=-0.01)
```
- **Evaluates:** `player.sGatheredStone > stoneCollectLimit`

---

### Resource Increment (Rate-of-Change)

#### ResourceIncrement
```cpp
ResourceIncrement(Constants::Resource resourceType, int amount,
                  int rewardSuccess=0.01, int rewardFailure=-0.01)
```
- **State:** Tracks `lastValue` between evaluations
- **Logic:**
  1. Check if `current > lastValue` (increment detected)
  2. Update `lastValue = current`
  3. Reward if increment occurred
- **Use:** Reward continuous progress, not just final goals

**Resource Types:**
- `Constants::Resource::Gold`
- `Constants::Resource::Lumber`
- `Constants::Resource::Stone`
- `Constants::Resource::Food`

---

### Unit Creation

#### UnitsCreated
```cpp
UnitsCreated(int unitsCreatedLimit, int rewardSuccess=0.01, int rewardFailure=-0.01)
```
- **Evaluates:** `player.sUnitsCreated >= unitsCreatedLimit`

#### NumUnitTypeCreated
```cpp
NumUnitTypeCreated(Constants::Unit unitType, int count,
                   int rewardSuccess=0.01, int rewardFailure=-0.01)
```
- **Evaluates:** `player.num_[unitType] >= count`
- **Supported Types:** Peasant, Peon, TownHall, Barracks, Footman, Farm, Archer

#### UnitIncrement
```cpp
UnitIncrement(Constants::Unit unitType, int amount,
              int rewardSuccess=0.01, int rewardFailure=-0.01)
```
- **State:** Tracks `lastValue` for specified unit type
- **Logic:** Same as ResourceIncrement but for unit counts

---

### Combat

#### DamageDone
```cpp
DamageDone(int damageDone, int rewardSuccess=0.01, int rewardFailure=-0.01)
```
- **Evaluates:** `player.sDamageDone >= damageDone`

#### DamageDoneIncrement
```cpp
DamageDoneIncrement(int amount, int rewardSuccess=0.01, int rewardFailure=-0.01)
```
- **State:** Tracks `lastValue`
- **Logic:** Rewards when damage dealt increases

#### DamageTaken
```cpp
DamageTaken(int damageTakenLimit, int rewardSuccess=0.01, int rewardFailure=-0.01)
```
- **Evaluates:** `player.sDamageTaken >= damageTakenLimit`
- **Note:** Can be used as penalty criterion with negative rewards

#### DamageTakenIncrement
```cpp
DamageTakenIncrement(int amount, int rewardSuccess=0.01, int rewardFailure=-0.01)
```
- **State:** Tracks `lastValue`
- **Use:** Penalize taking damage incrementally

---

### Food/Economy

#### FoodCount
```cpp
FoodCount(int foodCountLimit, int rewardSuccess=0.01, int rewardFailure=-0.01)
```
- **Evaluates:** `player.food >= foodCountLimit`
- **Use:** Track food production capacity

#### FoodConsumption
```cpp
FoodConsumption(int foodConsumptionLimit, int rewardSuccess=0.01, int rewardFailure=-0.01)
```
- **Evaluates:** `player.foodConsumption >= foodConsumptionLimit`
- **Use:** Track army size via food demand

---

### Game State

#### GameEnd
```cpp
GameEnd(int rewardSuccess=0.01, int rewardFailure=-0.01)
```
- **Logic:**
  - Reward if: `player.getState() == Constants::PlayerState::Playing`
  - Evaluate to true if: ALL players are in `Defeat` state
- **Use:** Terminal condition, rewards survival/victory

---

## Evaluation Flow

```
Scenario::evaluate(player)
    │
    ├── Get criteria list for player ID
    │
    └── std::all_of(criterias.begin(), criterias.end(),
            [&player](auto& c) { return c->evaluate(player); })

    Returns: true only if ALL criteria return true
```

## Reward Aggregation

```
Scenario::reward(player)
    │
    ├── Sum individual criterion rewards
    │
    └── Each criterion returns:
         - rewardSuccess (0.01) if evaluate() was true
         - rewardFailure (-0.01) if evaluate() was false
```

---

## Creating Custom Scenarios

### 1. Inherit from Scenario

```cpp
class MyScenario : public Scenario {
public:
    MyScenario()
        : Scenario(
            Config::Map::THELOR_31x31,
            Config().setStartGold(500).setBarracks(true),
            {
                std::make_shared<GoldCollect>(1000),
                std::make_shared<UnitsCreated>(5),
                std::make_shared<DamageDone>(100)
            },
            {} // optimal strategy (optional)
        ) {}
};
```

### 2. Use in Python

```python
from DeepRTS.python import scenario

# Built-in scenarios
env = scenario.GoldCollectFifteen()
env = scenario.GeneralAI_1v1(Config.Map.THIRTYONE)
env = scenario.LavaMaze()

# Custom via Python wrapper
class MyScenario(scenario.BaseScenario):
    def __init__(self):
        super().__init__(
            map_name="31x31-2p-ffa-Thelor.json",
            criterias=[
                GoldCollect(1000),
                UnitsCreated(10)
            ]
        )
```

---

## File Locations

| Component | Header | Implementation |
|-----------|--------|----------------|
| Scenario base | `include/DeepRTS/scenario/Scenario.h` | `src/scenario/Scenario.cpp` |
| ScenarioCriteria | `include/DeepRTS/scenario/criterias/ScenarioCriteria.h` | `src/scenario/criteria/ScenarioCriteria.cpp` |
| GoldCollectFifteen | `include/DeepRTS/scenario/scenarios/GoldCollectFifteen.h` | - |
| GeneralAIOneVersusOne | `include/DeepRTS/scenario/scenarios/GeneralAIOneVersusOne.h` | - |
| LavaMaze | `include/DeepRTS/scenario/scenarios/LavaMaze.h` | - |
| Individual criteria | `include/DeepRTS/scenario/criterias/` | `src/scenario/criteria/` |
