# https://github.com/Kitware/CMake/tree/master/Modules
# CMake projects should use -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake_minimum_required(VERSION 3.0)
project(DeepRTS)

# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)


set(CMAKE_CXX_STANDARD 14)


add_subdirectory(../include/pybind11 pybind11)


set(SOURCE_FILES
        # Engine
        src/main.cpp
        src/Game.cpp
        src/Game.h
        src/Constants.h
        src/Config.h

        # Player
        src/player/Player.cpp
        src/player/Player.h

        # Unit
        src/unit/Unit.cpp
        src/unit/Unit.h
        src/unit/UnitManager.h
        src/unit/UnitManager.cpp

        # Environment/Map
        src/environment/Tile.cpp
        src/environment/Tile.h
        src/environment/Tilemap.cpp
        src/environment/Tilemap.h
        src/environment/Map.cpp
        src/environment/Map.h


        # State
        src/state/BaseState.cpp
        src/state/BaseState.h
        src/state/Idle.cpp
        src/state/Idle.h
        src/state/Walking.cpp
        src/state/Walking.h
        src/state/Spawning.cpp
        src/state/Spawning.h
        src/state/StateManager.cpp
        src/state/StateManager.h
        src/state/Despawned.cpp
        src/state/Despawned.h
        src/state/Harvesting.cpp
        src/state/Harvesting.h
        src/state/Building.cpp
        src/state/Building.h
        src/state/Combat.cpp
        src/state/Combat.h
        src/state/Dead.cpp
        src/state/Dead.h
        src/state/StateManager.h
        src/state/StateManager.cpp


        src/util/ColorConverter.hpp
        src/util/Position.h
        src/util/Pathfinder.cpp
        src/util/Pathfinder.h
        src/util/PriorityQueue.hpp


        # Loaders
        src/loaders/ResourceLoader.cpp
        src/loaders/ResourceLoader.h
        src/Config.h)


add_executable(DeepRTS ${SOURCE_FILES})

target_link_libraries(DeepRTS PRIVATE pybind11::embed)

file(COPY ../assets DESTINATION ${CMAKE_BINARY_DIR})
