# https://github.com/Kitware/CMake/tree/master/Modules

cmake_minimum_required(VERSION 2.8.7)
project(DeepRTS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        c++/src/main.cpp
        c++/src/game/Game.cpp
        c++/src/game/Game.h
        c++/src/game/environment/Tile.cpp
        c++/src/game/environment/Tile.h
        c++/src/game/player/Player.cpp
        c++/src/game/player/Player.h
        c++/src/game/unit/Unit.cpp
        c++/src/game/unit/Unit.h
        c++/src/game/Config.h
        c++/src/game/state/Walking.cpp
        c++/src/game/state/Walking.h
        c++/src/game/state/Spawning.cpp
        c++/src/game/state/Spawning.h
        c++/src/game/state/StateManager.cpp
        c++/src/game/state/StateManager.h
        c++/src/game/Constants.h
        c++/src/game/state/BaseState.cpp
        c++/src/game/state/BaseState.h
        c++/src/game/state/Idle.cpp
        c++/src/game/state/Idle.h
        c++/src/game/environment/Tilemap.cpp
        c++/src/game/environment/Tilemap.h
        c++/src/game/state/Despawned.cpp
        c++/src/game/state/Despawned.h
        c++/src/game/graphics/Animation.cpp
        c++/src/game/graphics/Animation.h
        c++/src/game/graphics/GUI.cpp
        c++/src/game/graphics/GUI.h
        c++/src/game/state/Harvesting.cpp
        c++/src/game/state/Harvesting.h
        c++/src/game/util/Pathfinder.cpp
        c++/src/game/util/Pathfinder.h
        c++/src/game/util/PriorityQueue.hpp
        c++/src/game/state/Building.cpp
        c++/src/game/state/Building.h
        c++/src/game/util/ColorConverter.hpp
        c++/src/game/state/Combat.cpp
        c++/src/game/state/Combat.h
        c++/src/game/state/Dead.cpp
        c++/src/game/state/Dead.h
        c++/src/game/algorithms/base/Algorithm.h
        c++/src/game/algorithms/RANDOM/AlgoRandom.cpp
        c++/src/game/algorithms/RANDOM/AlgoRandom.h
        c++/src/game/algorithms/base/Algorithm.cpp
        c++/src/game/algorithms/base/AIRepository.cpp
        c++/src/game/algorithms/base/AIRepository.h
        c++/src/game/action/BaseAction.h
        c++/src/game/action/RightClickAction.cpp
        c++/src/game/action/RightClickAction.h

        c++/src/game/state/StateManager.h
        c++/src/game/state/StateManager.cpp
        c++/src/game/algorithms/PYAPI/PyAI.h
        c++/src/game/algorithms/PYAPI/PyAI.cpp
        c++/src/game/unit/UnitManager.h
        c++/src/game/unit/UnitManager.cpp
        c++/src/game/algorithms/MCTS/MCTS.cpp
        c++/src/game/algorithms/MCTS/MCTS.h
        c++/src/game/algorithms/MCTS/MCTSNode.cpp
        c++/src/game/algorithms/MCTS/MCTSNode.h
        c++/src/game/algorithms/MCAS/MCAS.cpp
        c++/src/game/algorithms/MCAS/MCAS.h
        c++/src/game/algorithms/MCAS/MCASNode.cpp
        c++/src/game/algorithms/MCAS/MCASNode.h
        c++/src/game/graphics/ResourceHandler.h
        c++/src/game/graphics/GraphicTile.cpp
        c++/src/game/graphics/GraphicTile.h
        c++/src/game/util/Position.h
        c++/src/game/graphics/GraphicTilemap.cpp
        c++/src/game/graphics/GraphicTilemap.h

        c++/src/game/graphics/plot/area.cpp
        c++/src/game/graphics/plot/area.h
        c++/src/game/graphics/plot/axis.cpp
        c++/src/game/graphics/plot/axis.h
        c++/src/game/graphics/plot/curve.cpp
        c++/src/game/graphics/plot/curve.h
        c++/src/game/graphics/plot/grid.cpp
        c++/src/game/graphics/plot/grid.h
        c++/src/game/graphics/plot/line.cpp
        c++/src/game/graphics/plot/line.h
        c++/src/game/graphics/plot/plot.cpp
        c++/src/game/graphics/plot/plot.h
        c++/src/game/algorithms/MCTSDirect/MCTSDirect.cpp
        c++/src/game/algorithms/MCTSDirect/MCTSDirect.h
        c++/src/game/loaders/ResourceLoader.cpp
        c++/src/game/loaders/ResourceLoader.h
        c++/src/game/logging/LogGame.h
        c++/src/game/logging/LogPlayer.h
        c++/src/game/logging/LogTick.h
        c++/src/game/logging/LogGame.cpp)

add_executable(DeepRTS ${SOURCE_FILES})


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake )
find_package(SFML 2 REQUIRED system window graphics network audio)
find_package(PythonLibs REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(Threads)


if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    include_directories(${FLATBUFFERS_INCLUDE_DIR})
    include_directories(${PYTHON_INCLUDE_DIR})
    target_link_libraries(DeepRTS ${FLATBUFFERS_STATIC_LIB})
    target_link_libraries(DeepRTS ${SFML_LIBRARIES})
    target_link_libraries(DeepRTS ${PYTHON_LIBRARIES})
    target_link_libraries(DeepRTS ${CMAKE_THREAD_LIBS_INIT})


else()
    message( FATAL_ERROR "You can not do this at all, CMake will exit. ${SFML_FOUND}" )
endif()


add_custom_command(
        TARGET DeepRTS PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/c++/src/game/data/
        ${CMAKE_CURRENT_BINARY_DIR}/data/)

add_custom_command(
        TARGET DeepRTS PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/c++/src/game/python/
        ${CMAKE_CURRENT_BINARY_DIR}/)


file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/games)

#file(COPY c++/src/game/data DESTINATION .) # ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ??
