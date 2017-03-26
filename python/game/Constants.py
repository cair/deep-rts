class Unit:
    Peasant = 1
    Peon = 2
    TownHall = 3
    Nothing = -1



class Race:
    Human = 1
    Orc = 2



class Tile:
    Spawn = 16
    Grass = 271
    Nothing = -1



class State:
    Idle = 8
    Spawning = 1
    Walking = 2
    Despawned = 3
    Harvesting = 4
    Building = 5
    Combat = 6
    Dead = 7
    Base = -1


class Pathfinding:
    Walkable = 1
    All = 2
    Attackable = 3
    Harvestable = 4


class Direction:
    Down = 1
    Up = 2
    Left = 3
    Right = 4
    DownLeft = 5
    DownRight = 6
    UpLeft = 7
    UpRight = 8


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
