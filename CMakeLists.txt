
cmake_minimum_required(VERSION 3.0)
project(Engine)

add_subdirectory(./include/pybind11 pybind11)

pybind11_add_module(Engine
        # Engine
        src/cplusplus/src/Game.cpp

        # Player
        src/cplusplus/src/player/Player.cpp

        # Environment
        src/cplusplus/src/environment/Map.cpp
        src/cplusplus/src/environment/Tile.cpp
        src/cplusplus/src/environment/Tilemap.cpp

        # Unit
        src/cplusplus/src/unit/Unit.cpp
        src/cplusplus/src/unit/UnitManager.cpp

        # State
        src/cplusplus/src/state/StateManager.cpp
        src/cplusplus/src/state/BaseState.cpp
        src/cplusplus/src/state/Walking.cpp
        src/cplusplus/src/state/Spawning.cpp
        src/cplusplus/src/state/Idle.cpp
        src/cplusplus/src/state/Despawned.cpp
        src/cplusplus/src/state/Harvesting.cpp
        src/cplusplus/src/state/Building.cpp
        src/cplusplus/src/state/Combat.cpp
        src/cplusplus/src/state/Dead.cpp

        # Util
        src/cplusplus/src/util/Pathfinder.cpp

        # Loaders
        src/cplusplus/src/loaders/ResourceLoader.cpp



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

