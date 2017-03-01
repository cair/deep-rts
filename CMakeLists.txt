# https://github.com/Kitware/CMake/tree/master/Modules

cmake_minimum_required(VERSION 3.6)
project(WarC2Sim__)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        main.cpp
        game/Game.cpp
        game/Game.h
        game/lib/csv.h
        game/lib/json.h
        game/loaders/MapLoader.cpp
        game/loaders/MapLoader.h
        game/environment/Tile.cpp
        game/environment/Tile.h
        game/player/Player.cpp
        game/player/Player.h
        game/unit/Unit.cpp
        game/unit/Unit.h
        game/Config.h
        game/unit/Peasant.cpp
        game/unit/Peasant.h
        game/state/Walking.cpp
        game/state/Walking.h
        game/state/Spawning.cpp
        game/state/Spawning.h
        game/state/StateManager.cpp
        game/state/StateManager.h
        game/Constants.h
        game/state/BaseState.cpp
        game/state/BaseState.h
        game/state/Idle.cpp
        game/state/Idle.h
        game/environment/Tilemap.cpp
        game/environment/Tilemap.h
        game/state/Despawned.cpp
        game/state/Despawned.h
        game/graphics/Animation.cpp
        game/graphics/Animation.h
        game/graphics/GUI.cpp
        game/graphics/GUI.h
        game/state/Harvesting.cpp
        game/state/Harvesting.h
        game/lib/Pathfinder.cpp
        game/lib/Pathfinder.h
        game/lib/PriorityQueue.h
        game/unit/TownHall.h
        game/unit/TownHall.cpp
        game/state/Building.cpp
        game/state/Building.h
        game/unit/InventoryManager.cpp
        game/unit/InventoryManager.h
        game/lib/ColorConverter.h
        game/state/Combat.cpp
        game/state/Combat.h
        game/state/Dead.cpp
        game/state/Dead.h
        server/Server.cpp
        server/Server.h
        game/proto/TestMessage.pb.cc
        game/proto/TestMessage.pb.h

        game/algorithms/base/Algorithm.h
        game/algorithms/RANDOM/AlgoRandom.cpp
        game/algorithms/RANDOM/AlgoRandom.h
        game/algorithms/base/Algorithm.cpp
        game/action/BaseAction.h game/action/RightClickAction.cpp game/action/RightClickAction.h)

add_executable(WarC2Sim__ ${SOURCE_FILES})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")



#include_directories(${PROTOBUF_INCLUDE_DIRS})
#include_directories(${CMAKE_CURRENT_BINARY_DIR})
#protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS foo.proto)
#add_executable(bar bar.cc ${PROTO_SRCS} ${PROTO_HDRS})
#target_link_libraries(bar ${PROTOBUF_LIBRARIES})


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





file(COPY game/data DESTINATION .) # ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ??