
cmake_minimum_required(VERSION 3.0)
project(Engine)

add_subdirectory(./include/pybind11 pybind11)

pybind11_add_module(Engine
        # Engine
        src/Game.cpp

        # Player
        src/player/Player.cpp

        # Environment
        src/environment/Map.cpp
        src/environment/Tile.cpp
        src/environment/Tilemap.cpp

        # Unit
        src/unit/Unit.cpp
        src/unit/UnitManager.cpp

        # State
        src/state/StateManager.cpp
        src/state/BaseState.cpp
        src/state/Walking.cpp
        src/state/Spawning.cpp
        src/state/Idle.cpp
        src/state/Despawned.cpp
        src/state/Harvesting.cpp
        src/state/Building.cpp
        src/state/Combat.cpp
        src/state/Dead.cpp

        # Util
        src/util/Pathfinder.cpp

        # Loaders
        src/loaders/ResourceLoader.cpp



       bindings/Constants.cpp
       bindings/BaseState.cpp
       bindings/UnitManager.cpp
       bindings/Unit.cpp
       bindings/Map.cpp
       bindings/Tile.cpp
       bindings/Tilemap.cpp
       bindings/Game.cpp
       bindings/Player.cpp
       bindings/Config.cpp
       bindings/DeepRTS.cpp
        )

