# https://github.com/Kitware/CMake/tree/master/Modules

cmake_minimum_required(VERSION 2.8.7)
project(DeepRTS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
set(CMAKE_CXX_STANDARD 14)


set(SOURCE_FILES
        src/main.cpp
        src/game/Game.cpp
        src/game/Game.h
        src/game/third_party/json.hpp
        src/game/loaders/MapLoader.cpp
        src/game/loaders/MapLoader.h
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
        src/game/algorithms/base/Algorithm.h
        src/game/algorithms/RANDOM/AlgoRandom.cpp
        src/game/algorithms/RANDOM/AlgoRandom.h
        src/game/algorithms/base/Algorithm.cpp
		src/game/algorithms/base/AIRepository.cpp
		src/game/algorithms/base/AIRepository.h
        src/game/action/BaseAction.h
        src/game/action/RightClickAction.cpp
        src/game/action/RightClickAction.h


        src/game/state/StateManager.h
		src/game/state/StateManager.cpp
		src/game/util/ScoreLogSerializer.h
		src/game/algorithms/PYAPI/PyAI.h
		src/game/algorithms/PYAPI/PyAI.cpp
		src/game/unit/UnitManager.h
		src/game/unit/UnitManager.cpp
		src/game/algorithms/MCTS/MCTS.cpp
		src/game/algorithms/MCTS/MCTS.h
		src/game/algorithms/MCTS/MCTSNode.cpp
		src/game/algorithms/MCTS/MCTSNode.h
		src/game/algorithms/MCAS/MCAS.cpp
		src/game/algorithms/MCAS/MCAS.h
		src/game/algorithms/MCAS/MCASNode.cpp
		src/game/algorithms/MCAS/MCASNode.h
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
        src/game/algorithms/MCTSDirect/MCTSDirect.cpp src/game/algorithms/MCTSDirect/MCTSDirect.h)

add_executable(DeepRTS ${SOURCE_FILES})


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake )
find_package(SFML 2 REQUIRED system window graphics network audio)
find_package(Flatbuffers REQUIRED)
find_package(PythonLibs)
find_package(Threads)


message(STATUS "Flatbuffers include dir: ${FLATBUFFERS_INCLUDE_DIR}")
message(STATUS "Flatbuffers static library: ${FLATBUFFERS_STATIC_LIB}")
message(STATUS "Flatbuffers compiler: ${FLATBUFFERS_COMPILER}")

add_custom_command(
        OUTPUT .
        WORKING_DIR ../
        COMMAND ./proto_compile.sh
)

execute_process (
        COMMAND bash -c "${CMAKE_SOURCE_DIR}/src/flatbuffers/ScoreLog/create_buffer.sh"
)

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
		${CMAKE_SOURCE_DIR}/src/game/data/
		${CMAKE_CURRENT_BINARY_DIR}/data/)

add_custom_command(
		TARGET DeepRTS PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/src/game/python/
		${CMAKE_CURRENT_BINARY_DIR}/)


file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/games)

#file(COPY src/game/data DESTINATION .) # ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ??
