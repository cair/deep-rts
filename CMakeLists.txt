# https://github.com/Kitware/CMake/tree/master/Modules
# CMake projects should use -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake_minimum_required(VERSION 2.8.7)
project(DeepRTS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") #  -std=c++1y
set(CMAKE_CXX_STANDARD 14)


set(SOURCE_FILES
        cplus/src/main.cpp
        cplus/src/game/Game.cpp
        cplus/src/game/Game.h
        cplus/src/game/environment/Tile.cpp
        cplus/src/game/environment/Tile.h
        cplus/src/game/player/Player.cpp
        cplus/src/game/player/Player.h
        cplus/src/game/unit/Unit.cpp
        cplus/src/game/unit/Unit.h
        cplus/src/game/Config.h
        cplus/src/game/state/Walking.cpp
        cplus/src/game/state/Walking.h
        cplus/src/game/state/Spawning.cpp
        cplus/src/game/state/Spawning.h
        cplus/src/game/state/StateManager.cpp
        cplus/src/game/state/StateManager.h
        cplus/src/game/Constants.h
        cplus/src/game/state/BaseState.cpp
        cplus/src/game/state/BaseState.h
        cplus/src/game/state/Idle.cpp
        cplus/src/game/state/Idle.h
        cplus/src/game/environment/Tilemap.cpp
        cplus/src/game/environment/Tilemap.h
        cplus/src/game/state/Despawned.cpp
        cplus/src/game/state/Despawned.h
        cplus/src/game/graphics/Animation.cpp
        cplus/src/game/graphics/Animation.h
        cplus/src/game/graphics/GUI.cpp
        cplus/src/game/graphics/GUI.h
        cplus/src/game/state/Harvesting.cpp
        cplus/src/game/state/Harvesting.h
        cplus/src/game/util/Pathfinder.cpp
        cplus/src/game/util/Pathfinder.h
        cplus/src/game/util/PriorityQueue.hpp
        cplus/src/game/state/Building.cpp
        cplus/src/game/state/Building.h
        cplus/src/game/util/ColorConverter.hpp
        cplus/src/game/state/Combat.cpp
        cplus/src/game/state/Combat.h
        cplus/src/game/state/Dead.cpp
        cplus/src/game/state/Dead.h
        cplus/src/game/algorithms/base/Algorithm.h
        cplus/src/game/algorithms/RANDOM/AlgoRandom.cpp
        cplus/src/game/algorithms/RANDOM/AlgoRandom.h
        cplus/src/game/algorithms/base/Algorithm.cpp
        cplus/src/game/algorithms/base/AIRepository.cpp
        cplus/src/game/algorithms/base/AIRepository.h
        cplus/src/game/action/BaseAction.h
        cplus/src/game/action/RightClickAction.cpp
        cplus/src/game/action/RightClickAction.h

        cplus/src/game/state/StateManager.h
        cplus/src/game/state/StateManager.cpp
        cplus/src/game/algorithms/PYAPI/PyAI.h
        cplus/src/game/algorithms/PYAPI/PyAI.cpp
        cplus/src/game/unit/UnitManager.h
        cplus/src/game/unit/UnitManager.cpp
        cplus/src/game/algorithms/MCTS/MCTS.cpp
        cplus/src/game/algorithms/MCTS/MCTS.h
        cplus/src/game/algorithms/MCTS/MCTSNode.cpp
        cplus/src/game/algorithms/MCTS/MCTSNode.h
        cplus/src/game/algorithms/MCAS/MCAS.cpp
        cplus/src/game/algorithms/MCAS/MCAS.h
        cplus/src/game/algorithms/MCAS/MCASNode.cpp
        cplus/src/game/algorithms/MCAS/MCASNode.h
        cplus/src/game/graphics/ResourceHandler.h
        cplus/src/game/graphics/GraphicTile.cpp
        cplus/src/game/graphics/GraphicTile.h
        cplus/src/game/util/Position.h
        cplus/src/game/graphics/GraphicTilemap.cpp
        cplus/src/game/graphics/GraphicTilemap.h

        cplus/src/game/graphics/plot/area.cpp
        cplus/src/game/graphics/plot/area.h
        cplus/src/game/graphics/plot/axis.cpp
        cplus/src/game/graphics/plot/axis.h
        cplus/src/game/graphics/plot/curve.cpp
        cplus/src/game/graphics/plot/curve.h
        cplus/src/game/graphics/plot/grid.cpp
        cplus/src/game/graphics/plot/grid.h
        cplus/src/game/graphics/plot/line.cpp
        cplus/src/game/graphics/plot/line.h
        cplus/src/game/graphics/plot/plot.cpp
        cplus/src/game/graphics/plot/plot.h
        cplus/src/game/algorithms/MCTSDirect/MCTSDirect.cpp
        cplus/src/game/algorithms/MCTSDirect/MCTSDirect.h
        cplus/src/game/loaders/ResourceLoader.cpp
        cplus/src/game/loaders/ResourceLoader.h
        cplus/src/game/logging/LogGame.h
        cplus/src/game/logging/LogPlayer.h
        cplus/src/game/logging/LogTick.h
        cplus/src/game/logging/LogGame.cpp)

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
        ${CMAKE_SOURCE_DIR}/cplus/src/game/data/
        ${CMAKE_CURRENT_BINARY_DIR}/data/)

add_custom_command(
        TARGET DeepRTS PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/cplus/src/game/python/
        ${CMAKE_CURRENT_BINARY_DIR}/)


file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/games)

#file(COPY c++/src/game/data DESTINATION .) # ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ??
