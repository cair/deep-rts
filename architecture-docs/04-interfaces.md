# Interfaces and APIs

> Complete specification of all external and internal interfaces.

## Python Public API

### Engine Module (`DeepRTS.Engine`)

Exposed via `bindings/DeepRTS.cpp`:

```python
from DeepRTS import Engine

# Classes
Engine.Game
Engine.Player
Engine.Config
Engine.Map
Engine.Tile
Engine.Tilemap
Engine.Unit
Engine.UnitManager
Engine.Constants
Engine.Random

# Submodules
Engine.scenario
Engine.scenario.criteria
```

### Game Class

**Source**: [bindings/Game.cpp](../bindings/Game.cpp)

```python
class Engine.Game:
    # Constructors
    def __init__(self, map_file: str): ...
    def __init__(self, map_file: str, config: Config): ...
    
    # Properties (read-only)
    config: Config
    state: np.ndarray      # Shape: (width, height, 10)
    units: List[Unit]
    _players: List[Player]
    tilemap: Tilemap
    map: Map
    selected_player: Player
    
    # Core Methods
    def tick(self) -> None: ...
    def update(self) -> None: ...
    def render(self) -> np.ndarray: ...
    def start(self) -> None: ...
    def stop(self) -> None: ...
    def reset(self) -> None: ...
    def is_terminal(self) -> bool: ...
    
    # Player Management
    def add_player(self) -> Player: ...
    def insert_player(self, player: Player) -> None: ...
    def set_selected_player(self, player: Player) -> None: ...
    
    # Getters
    def get_id(self) -> int: ...
    def get_episode(self) -> int: ...
    def get_ticks(self) -> int: ...
    def get_fps(self) -> int: ...
    def get_max_fps(self) -> int: ...
    def get_width(self) -> int: ...
    def get_height(self) -> int: ...
    def get_unit_by_name_id(self, name: str) -> Unit: ...
    
    # Setters
    def set_max_fps(self, fps: int) -> None: ...
    
    # Callbacks (override in subclass)
    def _on_unit_create(self, unit: Unit) -> None: ...
    def _on_unit_destroy(self, unit: Unit) -> None: ...
    def _on_episode_start(self) -> None: ...
    def _on_episode_end(self) -> None: ...
    def _on_tile_change(self, tile: Tile) -> None: ...
    def _on_resource_gather(self, tile: Tile, unit: Unit) -> None: ...
    def _on_resource_depleted(self, tile: Tile, unit: Unit) -> None: ...
```

### Player Class

**Source**: [bindings/Player.cpp](../bindings/Player.cpp)

```python
class Engine.Player:
    # Constructor
    def __init__(self, game: Game, id: int): ...
    
    # Resource Properties (read-only)
    gold: int
    lumber: int
    stone: int
    food: int
    food_consumption: int
    
    # Statistics (read-only)
    statistic_gathered_gold: int
    statistic_gathered_lumber: int
    statistic_gathered_stone: int
    statistic_damage_done: int
    statistic_damage_taken: int
    statistic_units_created: int
    
    # Unit Counts (read-only)
    num_peasant: int
    num_footman: int
    num_archer: int
    num_town_hall: int
    num_barracks: int
    num_farm: int
    
    # Methods
    def get_id(self) -> int: ...
    def get_name(self) -> str: ...
    def set_name(self, name: str) -> None: ...
    def get_score(self) -> int: ...
    
    # Actions
    def do_action(self, action: int) -> None: ...
    def do_manual_action(self, action: int, x: int, y: int) -> None: ...
    def right_click(self, x: int, y: int) -> None: ...
    def left_click(self, x: int, y: int) -> None: ...
    def spawn_unit(self, unit_type: Constants.Unit) -> None: ...
    
    # Unit Selection
    def get_targeted_unit(self) -> Unit: ...
    def set_targeted_unit_id(self, id: int) -> None: ...
```

### Constants

**Source**: [bindings/Constants.cpp](../bindings/Constants.cpp), [include/DeepRTS/Constants.h](../include/DeepRTS/Constants.h)

```python
class Engine.Constants:
    # Limits
    MAX_PLAYERS = 8
    MAX_UNITS = 1000
    action_min = 1
    action_max = 16
    
    # Enums exposed as module attributes
    class Unit:
        Peasant = 1
        Peon = 2
        TownHall = 3
        Barracks = 4
        Footman = 5
        Farm = 6
        Archer = 7
        None_ = -1
    
    class Action:
        PreviousUnit = 1
        NextUnit = 2
        MoveLeft = 3
        MoveRight = 4
        MoveUp = 5
        MoveDown = 6
        MoveUpLeft = 7
        MoveUpRight = 8
        MoveDownLeft = 9
        MoveDownRight = 10
        Attack = 11
        Harvest = 12
        Build0 = 13
        Build1 = 14
        Build2 = 15
        NoAction = 16
    
    class State:
        Spawning = 1
        Walking = 2
        Despawned = 3
        Harvesting = 4
        Building = 5
        Combat = 6
        Dead = 7
        Idle = 8
    
    class Resource:
        Lumber = 1
        Gold = 2
        Stone = 3
        Food = 4
    
    class Direction:
        Down = 1
        Up = 2
        Left = 3
        Right = 4
        DownLeft = 5
        DownRight = 6
        UpLeft = 7
        UpRight = 8
```

### Config Class

**Source**: [bindings/Config.cpp](../bindings/Config.cpp)

```python
class Engine.Config:
    @staticmethod
    def defaults() -> Config: ...
    
    # Game Mechanics
    tick_modifier: int = 10
    instant_town_hall: bool = True
    instant_building: bool = True
    harvest_forever: bool = False
    auto_attack: bool = True
    food_limit: int = 100
    
    # Unit Enablement
    farm_enabled: bool = True
    barracks_enabled: bool = True
    footman_enabled: bool = True
    archer_enabled: bool = False
    
    # Resources
    start_gold: int = 0
    start_lumber: int = 0
    start_stone: int = 0
    start_food: int = 1
    
    # Modifiers
    yield_modifier_gold: float = 1.0
    yield_modifier_lumber: float = 1.0
    yield_modifier_stone: float = 1.0
    tile_damage_modifier: float = 1.0
    
    # Display
    gui: str = "Blend2DGui"
    console_caption_enabled: bool = True
    animation_enabled: bool = True
    
    # Setters
    def set_gui(self, gui: str) -> None: ...
    def set_tick_modifier(self, mod: int) -> None: ...
    def set_terminal_signal(self, enabled: bool) -> None: ...
    # ... (see source for complete list)
```

### Scenario API

**Source**: [bindings/Scenarios.cpp](../bindings/Scenarios.cpp)

```python
# Scenario classes (extend Game)
class Engine.scenario.Scenario(Engine.Game):
    def evaluate(self, player: Player) -> bool: ...
    def reward(self, player: Player) -> int: ...
    def optimal_strategy(self) -> List[Tuple[Action, str]]: ...
    def compute_optimal_strategy(self, player: Player) -> Tuple[int, int, bool]: ...

class Engine.scenario.GoldCollectFifteen(Scenario): ...
class Engine.scenario.GeneralAIOneVersusOne(Scenario): ...

# Criteria classes
class Engine.scenario.criteria.ScenarioCriteria:
    def __init__(self, reward_success: int, reward_failure: int): ...
    def clone(self) -> ScenarioCriteria: ...
    def evaluate(self, player: Player) -> bool: ...
    def reward(self) -> int: ...

# Available criteria
Engine.scenario.criteria.DamageDone
Engine.scenario.criteria.DamageDoneIncrement
Engine.scenario.criteria.DamageTaken
Engine.scenario.criteria.DamageTakenIncrement
Engine.scenario.criteria.FoodConsumption
Engine.scenario.criteria.FoodCount
Engine.scenario.criteria.GoldCollect
Engine.scenario.criteria.LumberCollect
Engine.scenario.criteria.StoneCollect
Engine.scenario.criteria.UnitsCreated
Engine.scenario.criteria.UnitIncrement
Engine.scenario.criteria.NumUnitTypeCreated
Engine.scenario.criteria.ResourceIncrement
Engine.scenario.criteria.GameEnd
```

## Python Wrapper API

**Source**: [DeepRTS/python/game.py](../DeepRTS/python/game.py)

```python
from DeepRTS.python import Game, Config, GUI

class Game(Engine.Game):
    """Extended Game class with Gym-compatible interface."""
    
    def __init__(
        self,
        map_name: str,
        n_players: int = 2,
        engine_config: Engine.Config = None,
        gui_config: Config = None,
        tile_size: int = 32,
        terminal_signal: bool = False
    ): ...
    
    # Gym-compatible interface
    action_space: gym.spaces.Discrete
    observation_space: gym.spaces.Box
    
    @property
    def players(self) -> List[DeepRTSPlayer]: ...
    
    # State access
    def get_state(self, image: bool = False, copy: bool = True) -> np.ndarray: ...
    
    # Player control
    def set_player(self, player: DeepRTSPlayer) -> None: ...
    
    # Rendering control
    def set_render_frequency(self, interval: int) -> None: ...
    def set_view_every(self, n: int) -> None: ...
    def set_capture_every(self, n: int) -> None: ...
    
    # GUI
    def view(self) -> None: ...
    def capture(self) -> np.ndarray: ...
    def event(self) -> None: ...
```

## C++ Internal Interfaces

### BaseGUI Interface

**Source**: [include/DeepRTS/gui/BaseGUI.h](../include/DeepRTS/gui/BaseGUI.h)

```cpp
class BaseGUI {
protected:
    const Game& game;
public:
    cv::Mat renderData;
    
    explicit BaseGUI(Game& game);
    virtual const cv::Mat& render() const = 0;
    virtual void onTileChange(const Tile&) = 0;
};

// Implementations
class Blend2DGUI : public BaseGUI { ... };
```

### BaseState Interface

**Source**: [include/DeepRTS/state/BaseState.h](../include/DeepRTS/state/BaseState.h)

```cpp
class BaseState {
public:
    Constants::State id;
    std::string name;
    
    BaseState(Constants::State id);
    virtual ~BaseState() = 0;
    
    virtual void init(Unit& unit);
    virtual void update(Unit& unit);
    virtual void end(Unit& unit);
};
```

### Pathfinder Interface

**Source**: [include/DeepRTS/util/Pathfinder.h](../include/DeepRTS/util/Pathfinder.h)

```cpp
class Pathfinder {
public:
    static bool aStar(
        std::vector<Tile*>& path, 
        Tile* start, 
        Tile* goal
    );
    
    static Tile* find_first_walkable_tile(Tile* start);
    static Tile* find_first_harvestable_tile(Tile* start);
    static Tile* find_first_attackable_tile(Tile* start);
};
```

## Map File Format

**Location**: `DeepRTS/python/assets/maps/*.json`

```json
{
    "width": 10,
    "height": 10,
    "tile_width": 32,
    "tile_height": 32,
    "tiles": [
        [2, 2, 2, 6, 6, ...],
        [2, 1, 2, 2, 2, ...],
        ...
    ]
}
```

**Tile Type IDs**:
| ID | Type |
|----|------|
| 1 | Spawn |
| 2 | Grass |
| 3 | Wall |
| 4 | Desert |
| 5 | Water |
| 6 | Lumber |
| 7 | Gold |
| 8 | Stone |
| 9 | Lava |
