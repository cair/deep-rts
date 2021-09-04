cmake_minimum_required(VERSION 3.17)

project(DeepRTS VERSION 1.1.0 LANGUAGES CXX)

# If you want to use CUSTOM python, enable these. if not disable these :D
set(PYTHON_REQUIRED_VERSION 3.8)


# Includes
include(cmake/CMakeRC.cmake)
add_subdirectory(include/pybind11 pybind11)
add_subdirectory(include/random random)
add_subdirectory(include/spdlog spdlog)
add_subdirectory(include/drogon drogon)  # git submodule update --init --recursive
add_subdirectory(include/json json)


find_package(spdlog REQUIRED)

# Set variables
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra") # -Werror # TODO

# Creates Engine in DeepRTS package (instead of root)
if (NOT DEFINED PYTHON_BUILD)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/DeepRTS")
        find_package(Python ${PYTHON_REQUIRED_VERSION} REQUIRED COMPONENTS Interpreter Development)
endif ()

# Clang # TODO
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options(pybind11 INTERFACE -fsized-deallocation)
endif()

set(LIBRARIES
        effolkronium_random
        drogon
        nlohmann_json::nlohmann_json
        deeprts::rc
        spdlog::spdlog
        )

set(WEBSERVER_SOURCES
        src/contrib/webserver/Webserver.cpp
        src/contrib/webserver/Webserver.h
        src/contrib/webserver/Websockets.cpp
        src/contrib/webserver/Websockets.h
        )

set(BINDING_SOURCES
        bindings/Random.cpp
        bindings/Constants.cpp
        bindings/BaseState.cpp
        bindings/UnitManager.cpp
        bindings/Unit.cpp
        bindings/Map.cpp
        bindings/Tile.cpp
        bindings/Tilemap.cpp
        bindings/Game.cpp
        bindings/Player.cpp
        bindings/Config.cpp
        bindings/DeepRTS.cpp
        bindings/Webserver.cpp
        )

set(SOURCE_FILES
        # Engine
        src/main.cpp
        src/Game.cpp
        src/Game.h
        src/Constants.h
        src/Config.h
        src/loaders/ResourceLoader.cpp
        src/loaders/ResourceLoader.h

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
        src/environment/TileInfo.h

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

        # Utilities
        src/util/ColorConverter.hpp
        src/util/Position.h
        src/util/Pathfinder.cpp
        src/util/Pathfinder.h
        src/util/PriorityQueue.hpp
        src/util/Random.h
        src/util/String.h

        src/graphics/PyGUI.cpp
        src/graphics/PyGUI.h

        ${BINDING_SOURCES}
        ${WEBSERVER_SOURCES}
        )


pybind11_add_module(Engine
        ${SOURCE_FILES}
        bindings/Random.cpp
        bindings/Constants.cpp
        bindings/BaseState.cpp
        bindings/UnitManager.cpp
        bindings/Unit.cpp
        bindings/Map.cpp
        bindings/Tile.cpp
        bindings/Tilemap.cpp
        bindings/Game.cpp
        bindings/Player.cpp
        bindings/Config.cpp
        bindings/DeepRTS.cpp
        )

target_link_libraries(Engine PRIVATE ${LIBRARIES})

# CMRC
# Find All web files
FILE(GLOB_RECURSE web_resources RELATIVE ${PROJECT_SOURCE_DIR} src/contrib/webclient/dist/webclient/*)
cmrc_add_resource_library(DeepRTSResources ALIAS deeprts::rc
        WHENCE
        src/contrib
        ${web_resources}
)


# TODO hmmm is this good shit or do  i get bugs?
set_property(TARGET Engine PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET trantor PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET drogon PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET DeepRTSResources PROPERTY POSITION_INDEPENDENT_CODE ON)

# Dont create C++ Executable for Python builds (saves time)

if (NOT DEFINED PYTHON_BUILD)

        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/DeepRTS")  # Creates Engine in DeepRTS package (instead of root)
        add_executable(DeepRTSGame ${SOURCE_FILES})
        target_compile_definitions(DeepRTSGame PRIVATE EMBEDDED=1)
        target_compile_definitions(DeepRTSGame PRIVATE WEBDEV=1)
        target_link_libraries(DeepRTSGame PRIVATE
                pybind11::embed
                ${LIBRARIES}
        )

        # make python source the build dir.
        execute_process(
                COMMAND ln -sf ${PROJECT_SOURCE_DIR}/DeepRTS ${CMAKE_BINARY_DIR}
        )

endif ()


