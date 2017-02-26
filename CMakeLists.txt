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
        game/graphics/GUI.h)

add_executable(WarC2Sim__ ${SOURCE_FILES})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(WarC2Sim__ ${SFML_LIBRARIES})
endif()





file(COPY game/data DESTINATION .) # ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ??