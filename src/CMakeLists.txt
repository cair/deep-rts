# https://github.com/Kitware/CMake/tree/master/Modules
# CMake projects should use -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake_minimum_required(VERSION 3.0)
project(DeepRTS)

# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)


set(CMAKE_CXX_STANDARD 17)


add_subdirectory(../include/pybind11 pybind11)


set(SOURCE_FILES
        # Engine
        main.cpp
        Game.cpp
        Game.h
        Constants.h
        Config.h

        # Player
        player/Player.cpp
        player/Player.h

        # Unit
        unit/Unit.cpp
        unit/Unit.h
        unit/UnitManager.h
        unit/UnitManager.cpp

        # Environment/Map
        environment/Tile.cpp
        environment/Tile.h
        environment/Tilemap.cpp
        environment/Tilemap.h
        environment/Map.cpp
        environment/Map.h


        # State
        state/BaseState.cpp
        state/BaseState.h
        state/Idle.cpp
        state/Idle.h
        state/Walking.cpp
        state/Walking.h
        state/Spawning.cpp
        state/Spawning.h
        state/StateManager.cpp
        state/StateManager.h
        state/Despawned.cpp
        state/Despawned.h
        state/Harvesting.cpp
        state/Harvesting.h
        state/Building.cpp
        state/Building.h
        state/Combat.cpp
        state/Combat.h
        state/Dead.cpp
        state/Dead.h
        state/StateManager.h
        state/StateManager.cpp


        util/ColorConverter.hpp
        util/Position.h
        util/Pathfinder.cpp
        util/Pathfinder.h
        util/PriorityQueue.hpp


        # Loaders
        loaders/ResourceLoader.cpp
        loaders/ResourceLoader.h
        Config.h
        graphics/PyGUI.cpp
        graphics/PyGUI.h
        )


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(pybind11 INTERFACE -fsized-deallocation)
endif()

add_executable(DeepRTS ${SOURCE_FILES})

target_link_libraries(DeepRTS PRIVATE pybind11::embed)

file(COPY ../DeepRTS/python/assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ../DeepRTS/ DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ../requirements.txt DESTINATION ${CMAKE_BINARY_DIR})