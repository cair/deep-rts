# https://github.com/Kitware/CMake/tree/master/Modules
# CMake projects should use -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake_minimum_required(VERSION 2.8.7)
project(DeepRTS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_STANDARD 14)


set(SOURCE_FILES
        src/main.cpp
        src/game/Game.cpp
        src/game/Game.h
        src/game/environment/Tile.cpp
        src/game/environment/Tile.h
        src/game/player/Player.cpp
        src/game/player/Player.h
        src/game/unit/Unit.cpp
        src/game/unit/Unit.h
        src/game/Config.h
        src/game/state/Walking.cpp
        src/game/state/Walking.h
        src/game/state/Spawning.cpp
        src/game/state/Spawning.h
        src/game/state/StateManager.cpp
        src/game/state/StateManager.h
        src/game/Constants.h
        src/game/state/BaseState.cpp
        src/game/state/BaseState.h
        src/game/state/Idle.cpp
        src/game/state/Idle.h
        src/game/environment/Tilemap.cpp
        src/game/environment/Tilemap.h
        src/game/state/Despawned.cpp
        src/game/state/Despawned.h
        src/game/graphics/Animation.cpp
        src/game/graphics/Animation.h
        src/game/graphics/GUI.cpp
        src/game/graphics/GUI.h
        src/game/state/Harvesting.cpp
        src/game/state/Harvesting.h
        src/game/util/Pathfinder.cpp
        src/game/util/Pathfinder.h
        src/game/util/PriorityQueue.hpp
        src/game/state/Building.cpp
        src/game/state/Building.h
        src/game/util/ColorConverter.hpp
        src/game/state/Combat.cpp
        src/game/state/Combat.h
        src/game/state/Dead.cpp
        src/game/state/Dead.h

        src/game/action/BaseAction.h
        src/game/action/RightClickAction.cpp
        src/game/action/RightClickAction.h

        src/game/state/StateManager.h
        src/game/state/StateManager.cpp
        src/game/unit/UnitManager.h
        src/game/unit/UnitManager.cpp

        src/game/graphics/ResourceHandler.h
        src/game/graphics/GraphicTile.cpp
        src/game/graphics/GraphicTile.h
        src/game/util/Position.h
        src/game/graphics/GraphicTilemap.cpp
        src/game/graphics/GraphicTilemap.h

        src/game/graphics/plot/area.cpp
        src/game/graphics/plot/area.h
        src/game/graphics/plot/axis.cpp
        src/game/graphics/plot/axis.h
        src/game/graphics/plot/curve.cpp
        src/game/graphics/plot/curve.h
        src/game/graphics/plot/grid.cpp
        src/game/graphics/plot/grid.h
        src/game/graphics/plot/line.cpp
        src/game/graphics/plot/line.h
        src/game/graphics/plot/plot.cpp
        src/game/graphics/plot/plot.h
        src/game/loaders/ResourceLoader.cpp
        src/game/loaders/ResourceLoader.h
        src/game/logging/LogGame.h
        src/game/logging/LogPlayer.h
        src/game/logging/LogTick.h
        src/game/logging/LogGame.cpp)

add_executable(DeepRTS ${SOURCE_FILES})


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake )
find_package(SFML 2 REQUIRED system window graphics network audio)
find_package(PythonLibs REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(Threads)


if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    include_directories(${PYTHON_INCLUDE_DIR})
    target_link_libraries(DeepRTS ${SFML_LIBRARIES})
    target_link_libraries(DeepRTS ${UWS_LIBRARIES})
    target_link_libraries(DeepRTS ${ZLIB_LIBRARIES})
    target_link_libraries(DeepRTS ${OPENSSL_LIBRARIES})
    target_link_libraries(DeepRTS ${PYTHON_LIBRARIES})
    target_link_libraries(DeepRTS ${CMAKE_THREAD_LIBS_INIT})


else()
    message( FATAL_ERROR "You can not do this at all, CMake will exit. ${SFML_FOUND}" )
endif()


add_custom_command(
        TARGET DeepRTS PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data/
        ${CMAKE_CURRENT_BINARY_DIR}/data/)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/games)