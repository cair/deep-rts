# Data Flow

> Game loop execution, data flow, and sequence diagrams.

## Main Game Loop

```mermaid
flowchart TD
    subgraph Init["Initialization"]
        A[Create Game] --> B[Load Map]
        B --> C[Create Tilemap]
        C --> D[Initialize StateManager]
        D --> E[Add Players]
        E --> F[Spawn Initial Units]
        F --> G[game.start]
    end
    
    subgraph Loop["Game Loop"]
        G --> H{game.update}
        H --> I[tick - Update Clock]
        I --> J{Time to Update?}
        J -->|No| H
        J -->|Yes| K[Process All Units]
        K --> L[Check Terminal State]
        L --> M[Increment Ticks]
        M --> N{Terminal?}
        N -->|No| H
        N -->|Yes| O[Episode End]
    end
    
    subgraph Render["Render Path"]
        H -.-> P[game.render]
        P --> Q[GUI.render]
        Q --> R[cv::Mat Output]
    end
    
    O --> S[game.reset]
    S --> F
```

## Frame Update Sequence

```mermaid
sequenceDiagram
    participant App as Application
    participant Game
    participant Player
    participant Unit
    participant State as BaseState
    participant Tilemap
    
    App->>Game: update()
    Game->>Game: tick() [update clock]
    
    alt Time for Update
        loop For Each Unit
            Game->>Unit: update()
            Unit->>State: update(unit)
            State->>Unit: Modify state
            
            alt Walking
                Unit->>Tilemap: getNextTile()
                Tilemap-->>Unit: Tile*
                Unit->>Unit: setPosition()
            else Harvesting
                Unit->>Tilemap: getTile()
                Unit->>Unit: gatherResource()
            else Combat
                Unit->>Unit: getCombatTarget()
                Unit->>Unit: afflictDamage()
            end
            
            alt State Transition Needed
                Unit->>Unit: transitionState()
                Unit->>State: end(unit)
                Unit->>State: init(unit)
            end
        end
        
        Game->>Game: _update() [virtual hook]
        Game->>Game: ticks++
    end
    
    Game-->>App: return
```

## Action Processing Flow

```mermaid
sequenceDiagram
    participant Agent as ML Agent
    participant PyGame as Python Game
    participant Engine as C++ Engine
    participant Player
    participant Unit
    participant State
    
    Agent->>PyGame: env.step(action)
    PyGame->>Engine: player.do_action(action)
    
    Engine->>Player: do_action(action)
    
    alt Unit Selection (1-2)
        Player->>Player: nextUnit() / previousUnit()
    else Movement (3-10)
        Player->>Unit: getTargetedUnit()
        Unit->>Unit: tryMove(dx, dy)
        Unit->>State: transition to Walking
    else Attack (11)
        Player->>Unit: getTargetedUnit()
        Unit->>Unit: tryAttack()
        Unit->>State: transition to Combat
    else Harvest (12)
        Player->>Unit: getTargetedUnit()
        Unit->>Unit: tryHarvest()
        Unit->>State: transition to Harvesting
    else Build (13-15)
        Player->>Unit: getTargetedUnit()
        Unit->>Unit: build(idx)
        Unit->>State: transition to Building
    else NoAction (16)
        Note over Player: Do nothing
    end
    
    PyGame->>Engine: game.update()
    Engine-->>PyGame: return
    
    PyGame->>PyGame: get_state()
    PyGame->>PyGame: compute reward
    PyGame->>PyGame: check done
    
    PyGame-->>Agent: (next_state, reward, done, info)
```

## Resource Harvesting Flow

```mermaid
sequenceDiagram
    participant Unit
    participant Harvesting as HarvestingState
    participant Tile
    participant Player
    participant Game
    
    Unit->>Harvesting: update(unit)
    
    Harvesting->>Unit: Check harvestTimer
    
    alt Timer Complete
        Harvesting->>Tile: getResources()
        Tile-->>Harvesting: resource_count
        
        alt Resources Available
            Harvesting->>Unit: Add to carry (goldCarry/lumberCarry)
            Harvesting->>Tile: Decrement resources
            Harvesting->>Game: _onResourceGather(tile, unit)
            
            alt Tile Depleted
                Harvesting->>Tile: setDepleted()
                Harvesting->>Game: _onResourceDepleted(tile, unit)
                Harvesting->>Game: _onTileChange(tile)
            end
            
            alt Unit Carry Full
                Harvesting->>Unit: Find return location
                Harvesting->>Unit: transitionState(Walking)
            end
        else No Resources
            Harvesting->>Unit: transitionState(Idle)
        end
    end
```

## State Observation Format

The game state is an xtensor array with shape `(width, height, 10)`:

```
State Tensor: (W x H x 10)
├── Channel 0: Tile type ID
├── Channel 1: Tile walkability
├── Channel 2: Tile resources remaining
├── Channel 3: Unit presence (unit ID or -1)
├── Channel 4: Unit type
├── Channel 5: Unit owner (player ID)
├── Channel 6: Unit health
├── Channel 7: Unit state
├── Channel 8: Reserved
└── Channel 9: Reserved
```

## Rendering Pipeline

```mermaid
flowchart LR
    subgraph C++["C++ Rendering (Blend2D)"]
        A[Game State] --> B[Blend2DGUI]
        B --> C[Draw Tiles]
        C --> D[Draw Units]
        D --> E[Draw Effects]
        E --> F[cv::Mat]
    end
    
    subgraph Python["Python Rendering (Pygame)"]
        F --> G[NDArray Converter]
        G --> H[NumPy Array]
        H --> I[Pygame Surface]
        I --> J[Draw Health Bars]
        J --> K[Draw UI]
        K --> L[Display/Capture]
    end
```

## Episode Lifecycle

```mermaid
stateDiagram-v2
    [*] --> Created: Game()
    Created --> Started: start()
    
    Started --> Running: First update()
    Running --> Running: update() loop
    Running --> Terminal: isTerminal() == true
    
    Terminal --> EpisodeEnd: _onEpisodeEnd()
    EpisodeEnd --> Reset: reset()
    
    Reset --> Started: New episode
    
    Running --> Stopped: stop()
    Stopped --> [*]
```

## Data Flow Between Layers

```
┌─────────────────────────────────────────────────────────────┐
│                     Python Layer                             │
│  ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐  │
│  │  Agent  │───▶│env.step │───▶│get_state│───▶│  Model  │  │
│  └─────────┘    └─────────┘    └─────────┘    └─────────┘  │
│       │              │              ▲                        │
│       │         action (int)        │ np.ndarray             │
│       │              │              │                        │
└───────┼──────────────┼──────────────┼────────────────────────┘
        │              │              │
        │              ▼              │
┌───────┼────────────────────────────┼─────────────────────────┐
│       │        Binding Layer       │                         │
│       │    ┌───────────────────────┴───┐                     │
│       │    │ xtensor ←→ numpy converter │                     │
│       │    └───────────────────────────┘                     │
│       │              │                                       │
└───────┼──────────────┼───────────────────────────────────────┘
        │              │
        │              ▼
┌───────┼──────────────────────────────────────────────────────┐
│       │          C++ Engine Layer                            │
│       ▼                                                      │
│  ┌─────────┐    ┌─────────┐    ┌─────────┐                  │
│  │do_action│───▶│ update  │───▶│  state  │                  │
│  └─────────┘    └─────────┘    └─────────┘                  │
│                      │              │                        │
│                      ▼              │                        │
│                ┌─────────┐          │                        │
│                │ render  │          │                        │
│                └─────────┘          │                        │
│                      │              │                        │
│                cv::Mat              │ xt::xarray             │
└──────────────────────┴──────────────┴────────────────────────┘
```
