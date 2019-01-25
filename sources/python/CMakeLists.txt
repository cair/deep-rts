
cmake_minimum_required(VERSION 3.0)
project(pyDeepRTS)

add_subdirectory(../cplusplus/include/pybind11 pybind11)

pybind11_add_module(pyDeepRTS
        # Engine
        ../cplusplus/src/Game.cpp

        # Player
        ../cplusplus/src/player/Player.cpp

        # Environment
        ../cplusplus/src/environment/Map.cpp
        ../cplusplus/src/environment/Tile.cpp
        ../cplusplus/src/environment/Tilemap.cpp

        # Unit
        ../cplusplus/src/unit/Unit.cpp
        ../cplusplus/src/unit/UnitManager.cpp

        # State
        ../cplusplus/src/state/StateManager.cpp
        ../cplusplus/src/state/BaseState.cpp
        ../cplusplus/src/state/Walking.cpp
        ../cplusplus/src/state/Spawning.cpp
        ../cplusplus/src/state/Idle.cpp
        ../cplusplus/src/state/Despawned.cpp
        ../cplusplus/src/state/Harvesting.cpp
        ../cplusplus/src/state/Building.cpp
        ../cplusplus/src/state/Combat.cpp
        ../cplusplus/src/state/Dead.cpp

        # Util
        ../cplusplus/src/util/Pathfinder.cpp

        # Loaders
        ../cplusplus/src/loaders/ResourceLoader.cpp


        bindings/DeepRTS.cpp
        bindings/Constants.cpp
        bindings/Unit.cpp
        bindings/Map.cpp
        bindings/Tile.cpp
        bindings/Tilemap.cpp
        bindings/Game.cpp
        bindings/Player.cpp
        bindings/DeepRTS.cpp
        bindings/Config.cpp

        )
