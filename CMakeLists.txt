# Project Name
project(DeepRTS)

# Cmake version
cmake_minimum_required(VERSION 3.0)



# Creates Engine in DeepRTS package (instead of root)
if (NOT DEFINED PYTHON_BUILD)
        find_package(PythonInterp 3.7 REQUIRED) # Todo
        find_package(PythonLibs 3.7 REQUIRED) # todo - UPGRADE
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/DeepRTS")
endif ()

# -DCMAKE_BUILD_TYPE=Release

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(PYBIND11_CPP_STANDARD -std=c++1z) # Experimental C++17 support

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra") # -Werror
add_subdirectory(./include/pybind11 pybind11)

# Clang # TODO
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options(pybind11 INTERFACE -fsized-deallocation)
endif()

set(SOURCE_FILES
        # Engine
        src/main.cpp
        src/Game.cpp
        src/Game.h
        src/Constants.h
        src/Config.h
        src/loaders/ResourceLoader.cpp
        src/loaders/ResourceLoader.h

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

        # Utilities
        src/util/ColorConverter.hpp
        src/util/Position.h
        src/util/Pathfinder.cpp
        src/util/Pathfinder.h
        src/util/PriorityQueue.hpp
        src/util/Random.h

        src/graphics/PyGUI.cpp
        src/graphics/PyGUI.h

        )


pybind11_add_module(Engine
        ${SOURCE_FILES}
        bindings/Random.cpp
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


# Dont create C++ Executable for Python builds (saves time)

if (NOT DEFINED PYTHON_BUILD)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/DeepRTS")  # Creates Engine in DeepRTS package (instead of root)
        add_executable(DeepRTSGame ${SOURCE_FILES})
        target_link_libraries(DeepRTSGame PRIVATE pybind11::embed)
        file(COPY ./DeepRTS DESTINATION ${CMAKE_BINARY_DIR})
endif ()


