# https://github.com/Kitware/CMake/tree/master/Modules

cmake_minimum_required(VERSION 3.6)
project(WarC2Sim__)

set(CMAKE_CXX_STANDARD 14)


set(SOURCE_FILES
        src/main.cpp
        src/game/Game.cpp
        src/game/Game.h
        src/game/lib/csv.h
        src/game/lib/json.h
        src/game/loaders/MapLoader.cpp
        src/game/loaders/MapLoader.h
        src/game/environment/Tile.cpp
        src/game/environment/Tile.h
        src/game/player/Player.cpp
        src/game/player/Player.h
        src/game/unit/Unit.cpp
        src/game/unit/Unit.h
        src/game/Config.h
        src/game/unit/Peasant.cpp
        src/game/unit/Peasant.h
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
        src/game/lib/Pathfinder.cpp
        src/game/lib/Pathfinder.h
        src/game/lib/PriorityQueue.h
        src/game/unit/TownHall.h
        src/game/unit/TownHall.cpp
        src/game/state/Building.cpp
        src/game/state/Building.h
        src/game/unit/InventoryManager.cpp
        src/game/unit/InventoryManager.h
        src/game/lib/ColorConverter.h
        src/game/state/Combat.cpp
        src/game/state/Combat.h
        src/game/state/Dead.cpp
        src/game/state/Dead.h

        src/game/proto/TestMessage.pb.cc
        src/game/proto/TestMessage.pb.h

        src/game/proto/GameMessage.pb.cc
        src/game/proto/GameMessage.pb.h

        src/game/algorithms/base/Algorithm.h
        src/game/algorithms/RANDOM/AlgoRandom.cpp
        src/game/algorithms/RANDOM/AlgoRandom.h
        src/game/algorithms/base/Algorithm.cpp
        src/game/action/BaseAction.h
        src/game/action/RightClickAction.cpp
        src/game/action/RightClickAction.h
        src/server/Server.cpp
        src/server/Server.h
        src/server/ServerWorker.h
        src/server/ServerTask.h)

add_executable(WarC2Sim__ ${SOURCE_FILES})


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake )


include(UseCython)


find_package(SFML REQUIRED system window graphics network audio)
find_package(ZeroMQ REQUIRED)
find_package(Protobuf REQUIRED)


add_custom_command(
        OUTPUT .
        WORKING_DIR ../
        COMMAND ./proto_compile.sh
)

if (SFML_FOUND AND ZEROMQ_FOUND AND PROTOBUF_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
    include_directories(${PROTOBUF_INCLUDE_DIRS})
    include_directories(${ZeroMQ_INCLUDE_DIR})

    #protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS proto/SearchRequest.proto)
    target_link_libraries(WarC2Sim__ ${PROTOBUF_LIBRARIES})
    target_link_libraries(WarC2Sim__ ${SFML_LIBRARIES})
    target_link_libraries(WarC2Sim__ ${ZeroMQ_LIBRARIES})



else()
    message( FATAL_ERROR "You can not do this at all, CMake will exit. ${SFML_FOUND} and ${ZEROMQ_FOUND}." )
endif()





file(COPY src/game/data DESTINATION .) # ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ??