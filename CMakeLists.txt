# https://github.com/Kitware/CMake/tree/master/Modules
# CMake projects should use -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake_minimum_required(VERSION 2.8.7)
project(DeepRTS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake)

find_package(SFML 2 COMPONENTS system window graphics network audio)


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
        )

if(SFML_FOUND)
    set(SOURCE_FILES
            # Graphical Interface (C++)
            ${SOURCE_FILES}
            src/graphics/GUIGame.cpp
            src/graphics/GUIGame.h
            src/graphics/Animation.cpp
            src/graphics/Animation.h
            src/graphics/GraphicTile.cpp
            src/graphics/GraphicTile.h
            src/graphics/GraphicTilemap.cpp
            src/graphics/GraphicTilemap.h
            src/graphics/GUI.cpp
            src/graphics/GUI.h
            src/graphics/ResourceHandler.h
            )
endif()

add_executable(DeepRTS ${SOURCE_FILES})








if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(DeepRTS ${SFML_LIBRARIES})
elseif()
    message( FATAL_ERROR "You can not do this at all, CMake will exit. ${SFML_FOUND}" )
endif()







add_custom_command(
        TARGET DeepRTS PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data/
        ${CMAKE_CURRENT_BINARY_DIR}/data/)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/games)