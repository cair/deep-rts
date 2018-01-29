# https://github.com/Kitware/CMake/tree/master/Modules
# CMake projects should use -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake_minimum_required(VERSION 2.8.7)
project(DeepRTS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_STANDARD 14)


set(SOURCE_FILES
        src/main.cpp
        src/Game.cpp
        src/Game.h
        src/environment/Tile.cpp
        src/environment/Tile.h
        src/player/Player.cpp
        src/player/Player.h
        src/unit/Unit.cpp
        src/unit/Unit.h
        src/Config.h
        src/state/Walking.cpp
        src/state/Walking.h
        src/state/Spawning.cpp
        src/state/Spawning.h
        src/state/StateManager.cpp
        src/state/StateManager.h
        src/Constants.h
        src/state/BaseState.cpp
        src/state/BaseState.h
        src/state/Idle.cpp
        src/state/Idle.h
        src/environment/Tilemap.cpp
        src/environment/Tilemap.h
        src/state/Despawned.cpp
        src/state/Despawned.h
        src/state/Harvesting.cpp
        src/state/Harvesting.h
        src/util/Pathfinder.cpp
        src/util/Pathfinder.h
        src/util/PriorityQueue.hpp
        src/state/Building.cpp
        src/state/Building.h
        src/util/ColorConverter.hpp
        src/state/Combat.cpp
        src/state/Combat.h
        src/state/Dead.cpp
        src/state/Dead.h
        src/action/BaseAction.h
        src/action/RightClickAction.cpp
        src/action/RightClickAction.h
        src/state/StateManager.h
        src/state/StateManager.cpp
        src/unit/UnitManager.h
        src/unit/UnitManager.cpp
        src/util/Position.h
        src/loaders/ResourceLoader.cpp
        src/loaders/ResourceLoader.h
        src/logging/LogGame.h
        src/logging/LogPlayer.h
        src/logging/LogTick.h
        src/logging/LogGame.cpp
        )

add_executable(DeepRTS ${SOURCE_FILES})


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake )
find_package(PythonLibs REQUIRED)
find_package(Threads)



include_directories(${PYTHON_INCLUDE_DIR})
target_link_libraries(DeepRTS ${PYTHON_LIBRARIES})
target_link_libraries(DeepRTS ${CMAKE_THREAD_LIBS_INIT})




add_custom_command(
        TARGET DeepRTS PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data/
        ${CMAKE_CURRENT_BINARY_DIR}/data/)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/games)