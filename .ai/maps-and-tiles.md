# DeepRTS Maps and Tiles System

## Overview

The world is represented as a 2D grid of tiles, loaded from JSON map files with properties defined in a central configuration.

## Architecture

```
Map (JSON loader)
 │
 ├── tileIDs[]           ──► Raw tile IDs from JSON
 ├── tilesData{}         ──► TileData lookup by ID
 │
 └──► Tilemap (Grid manager)
      │
      ├── tiles[]        ──► Linear array of Tile objects
      │                      Index: MAP_WIDTH * y + x
      │
      └── spawnTiles[]   ──► Spawn point tiles
```

## Tile Types

| Type | ID Range | Walkable | Harvestable | Resources | Walk Speed | Damage |
|------|----------|----------|-------------|-----------|------------|--------|
| **Spawn** | 16 | No | No | 0 | - | 0 |
| **Wall** | 17-87 | No | No | 0 | - | 0 |
| **Gold** | 88-101 | No | Yes | 1000 | - | 0 |
| **Lumber** | 102-141 | No | Yes | 1000 | - | 0 |
| **Stone** | 142-179 | No | Yes | 1000 | - | 0 |
| **Water** | 300-333 | No | No | 0 | - | 0 |
| **Desert** | 180-205, 334-355 | Yes | No | 0 | 0.5x | 0 |
| **Grass** | 238-269, 356-371 | Yes | No | 0 | 1.0x | 0 |
| **Lava** | 372 | Yes | No | 0 | 0.5x | 1.0 |

## TileData Structure

```cpp
struct TileData {
    int depleteTile;        // Tile ID when harvested
    std::string name;       // Type name ("Grass", "Gold", etc.)
    bool walkable;          // Can units traverse
    bool harvestable;       // Can be harvested
    double walk_modifier;   // Speed multiplier (1.0 = normal)
    int resources;          // Total harvestable amount
    double lumber_yield;    // Lumber per harvest action
    double gold_yield;      // Gold per harvest action
    double stone_yield;     // Stone per harvest action
    double damage_modifier; // Damage per tick to occupants
};
```

## Resource Yields

| Tile Type | Lumber | Gold | Stone |
|-----------|--------|------|-------|
| Lumber | 1.0 | 0 | 0 |
| Gold | 0 | 1.0 | 0 |
| Stone | 0 | 0 | 1.0 |
| Others | 0 | 0 | 0 |

## Depletion System

Each tile stores **dual state** (pre-harvest and post-harvest):

```
BEFORE HARVEST (Lumber tile)          AFTER HARVEST (depleted)
─────────────────────────────────────────────────────────────
typeId: 102                           typeId: 269
name: "Lumber"                        name: "Grass"
walkable: false                       walkable: true
harvestable: true                     harvestable: false
resources: 1000                       resources: 0
walkModifier: 0                       walkModifier: 1.0
```

### Depletion Flow

```cpp
void takeResource(uint8_t n) {
    resources -= n;
    if (resources <= 0) {
        setDepleted();  // Transition to depleted state
    }
}
```

## Available Maps

| Map | Size | Players | Type | Features |
|-----|------|---------|------|----------|
| `10x10-2p-ffa-Eblil.json` | 10x10 | 2 | FFA | Small, competitive |
| `15x15-1p-ffa-Utha-Lava.json` | 15x15 | 1 | FFA | Lava hazards |
| `15x15-2p-ffa-Cresal.json` | 15x15 | 2 | FFA | Medium size |
| `21x21-1p-scenario-Gondor.json` | 21x21 | 1 | Scenario | Single-player |
| `21x21-2p-ffa-Eflines.json` | 21x21 | 2 | FFA | Larger 2P |
| `21x21-2p-ffa-FindGold-Vastrana.json` | 21x21 | 2 | FFA | Gold-focused |
| `30x30-4p-ffa-Bloilor-Lava.json` | 30x30 | 4 | FFA | Large with lava |
| `31x31-2p-ffa-Thelor.json` | 31x31 | 2 | FFA | Largest 2P |
| `31x31-4p-ffa-Thelor.json` | 31x31 | 4 | FFA | 4-player variant |
| `31x31-6p-ffa-Thelor.json` | 31x31 | 6 | FFA | 6-player variant |

## Map JSON Format

```json
{
  "height": 31,
  "width": 31,
  "tileheight": 32,
  "tilewidth": 32,
  "layers": [{
    "name": "Rutelag 1",
    "data": [142, 270, 102, ...],  // 1-indexed tile IDs
    "height": 31,
    "width": 31,
    "type": "tilelayer"
  }],
  "tilesets": [{
    "tilewidth": 32,
    "tileheight": 32,
    "tilecount": 380,
    "columns": 19,
    "image": "../textures/tiles.png"
  }],
  "orientation": "orthogonal",
  "renderorder": "right-down"
}
```

**Note:** JSON tile IDs are 1-indexed. The loader subtracts 1 for 0-indexed internal use.

## Tile Properties File

**File:** `DeepRTS/python/assets/tile_properties.json`

```json
{
  "102": {
    "deplete_tile": 269,
    "name": "Lumber",
    "walkable": false,
    "harvestable": true,
    "walk_modifier": 0,
    "resources": 1000,
    "lumber_yield": 1.0,
    "gold_yield": 0,
    "stone_yield": 0,
    "damage_modifier": 0
  },
  ...
}
```

## Tile Class

### Properties

| Property | Type | Description |
|----------|------|-------------|
| `id` | int | Tile index in tilemap |
| `x`, `y` | int | Grid coordinates |
| `typeId` | int | Current tile type ID |
| `name` | string | Type name |
| `walkable` | bool | Traversable by units |
| `harvestable` | bool | Contains resources |
| `resources` | int | Resources remaining |
| `depleted` | bool | Fully harvested |
| `walkModifier` | double | Speed multiplier |
| `damageModifier` | double | Damage per tick |
| `occupantID` | int | Unit on tile (-1 if none) |

### Query Methods

```cpp
bool isWalkable() const;     // walkable AND no occupant
bool isHarvestable() const;
bool isBuildable() const;    // Alias for isWalkable
bool isDepleted() const;
bool hasOccupant() const;
int getResources() const;
int distance(Tile &target) const;  // Euclidean distance
```

### Yield Methods

```cpp
double getLumberYield() const;
double getGoldYield() const;
double getStoneYield() const;
double getWalkModifier() const;
double getDamageModifier() const;
```

### Modification Methods

```cpp
void takeResource(uint8_t n);      // Harvest n resources
void setResources(uint16_t count);
void setDepleted();                // Force depletion
void reset();                      // Restore original state
void setOccupant(Unit *unit);
```

## Tilemap Class

### Access Methods

```cpp
Tile& getTile(uint32_t x, uint32_t y);
std::vector<Tile*> getTileArea(Tile &source, uint32_t width, uint32_t height);
Tile& getSpawnTile(int playerID);
```

### Neighbor Queries

```cpp
std::vector<Tile*> neighbors(Tile &tile, Constants::Pathfinding type);
```

**Pathfinding Types:**
- `Walkable` - Only walkable tiles
- `All` - All 8 neighbors
- `Attackable` - Tiles with enemy units
- `Harvestable` - Tiles with resources

### Pathfinding Support

```cpp
// Used by JPS pathfinder
bool operator()(unsigned x, unsigned y) const;  // Returns isWalkable
```

### Reset

```cpp
void reset();  // Reset all tiles to original state
```

## Environmental Damage

Lava tiles deal damage to occupants:

```cpp
void Tile::update(Unit& unit) const {
    if (hasOccupant() && damageModifier > 0) {
        unit.afflictDamage(
            ceil(damageModifier * game.config.getTileDamageModifier())
        );
    }
}
```

Configure via `Config::setTileDamageModifier(double)`.

## Map Loading Process

1. **ResourceLoader** reads embedded JSON (via CMRC)
2. **Map** parses JSON structure:
   - Extracts tile IDs from `layers[0].data`
   - Loads tile properties from `tile_properties.json`
3. **Tilemap** creates grid:
   - Allocates `width * height` tiles
   - For each position, creates Tile with both states
   - Identifies spawn tiles for player placement

## Spawn Points

Tiles with name "Spawn" (ID 16) are stored separately:

```cpp
std::vector<Tile*> spawnTiles;

Tile& getSpawnTile(int playerID) {
    return *spawnTiles[playerID];
}
```

Each player gets assigned a spawn tile at initialization.

## File Locations

| Component | Path |
|-----------|------|
| Map class | `include/DeepRTS/Map.h`, `src/Map.cpp` |
| Tile class | `include/DeepRTS/Tile.h`, `src/Tile.cpp` |
| Tilemap class | `include/DeepRTS/Tilemap.h`, `src/Tilemap.cpp` |
| Map files | `DeepRTS/python/assets/maps/` |
| Tile properties | `DeepRTS/python/assets/tile_properties.json` |
| Textures | `DeepRTS/python/assets/textures/tiles.png` |
