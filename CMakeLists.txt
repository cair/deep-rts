# https://github.com/Kitware/CMake/tree/master/Modules

cmake_minimum_required(VERSION 3.5)
project(WarC2Sim__)

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
        src/game/action/BaseAction.h
        src/game/action/RightClickAction.cpp
        src/game/action/RightClickAction.h
#        src/server/Server.cpp
#        src/server/Server.h
#        src/server/ServerWorker.h
#        src/server/ServerTask.h
        src/game/state/StateManager.h
	src/game/state/StateManager.cpp
	src/game/util/ScoreLogSerializer.h
	src/game/algorithms/PYAPI/PyAPI.h
	src/game/algorithms/PYAPI/PyAPI.cpp
	src/game/unit/UnitManager.h
	src/game/unit/UnitManager.cpp
        src/game/algorithms/MCTS/MCTS.cpp
	src/game/algorithms/MCTS/MCTS.h
	src/game/algorithms/MCTS/MCTSNode.cpp
	src/game/algorithms/MCTS/MCTSNode.h)

add_executable(WarC2Sim__ ${SOURCE_FILES})


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

if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    include_directories(${FLATBUFFERS_INCLUDE_DIR})
    include_directories(${PYTHON_INCLUDE_DIR})
    target_link_libraries(WarC2Sim__ ${FLATBUFFERS_STATIC_LIB})
    target_link_libraries(WarC2Sim__ ${SFML_LIBRARIES})
    target_link_libraries(WarC2Sim__ ${PYTHON_LIBRARIES})
    target_link_libraries(WarC2Sim__ ${CMAKE_THREAD_LIBS_INIT})


else()
    message( FATAL_ERROR "You can not do this at all, CMake will exit. ${SFML_FOUND}" )
endif()





file(COPY src/game/data DESTINATION .) # ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ??
