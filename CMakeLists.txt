cmake_minimum_required(VERSION 3.17)
project(DeepRTS VERSION 1.1.0 LANGUAGES CXX)

# Includes
include(cmake/CMakeRC.cmake)
include(cmake/CPM.cmake)
include(cmake/pmm.cmake)

#############################################################################################
#####
##### C++ Compiler Setup
#####
#############################################################################################
set(CMAKE_CXX_STANDARD 20)
set(PYBIND11_CPP_STANDARD /std:c++20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)


if (LIBRARY_TYPE STREQUAL "SHARED")
        set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif ()

if (UNIX)
        set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
endif ()


if (CMAKE_BUILD_TYPE EQUAL "RELEASE")
        if (WIN32)
                add_compile_options(/Ox /utf-8)
        else ()
                add_compile_options(-Ofast -march=native -ffast-math -Wall)
        endif ()
endif ()


if (IS_PYBIND_BUILD)
        set(LIBRARY_TYPE SHARED)
elseif (WIN32)
        set(LIBRARY_TYPE STATIC)
endif ()


#############################################################################################
#####
##### VCPKG - Configuration
#####
#############################################################################################
if (WIN32)
        set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "" FORCE)  # -static ?
        set(VCPKG_STATIC_LINKING ON)
elseif (UNIX)
        set(VCPKG_TARGET_TRIPLET x64-linux)
endif ()

#############################################################################################
#####
##### VCPKG - Dependencies
#####
#############################################################################################
set(DEEPRTS_DEPENDENCIES
        spdlog
        effolkronium-random
        python3
        xtensor[xsimd]
        pybind11
        drogon
        nlohmann-json
        )



set(DEEPRTS_LINK_TARGETS
        DeepRTSResources
        spdlog::spdlog
        effolkronium_random
        Python3::NumPy
        xtensor::optimize
        xtensor::use_xsimd
        #blend2d
        drogon
)


# VCPKG - Install Depedencies
pmm(
        VERBOSE
        DEBUG
        VCPKG
        REVISION master
        REQUIRES ${DEEPRTS_DEPENDENCIES}
        TRIPLET ${VCPKG_TARGET_TRIPLET}
)

#############################################################################################
#####
##### CMAKE - Find Packages
#####
#############################################################################################
find_package(spdlog CONFIG REQUIRED)
find_package(Python3 COMPONENTS Interpreter NumPy REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(effolkronium_random CONFIG REQUIRED)
find_package(xtensor CONFIG REQUIRED)

find_package(nlohmann_json CONFIG REQUIRED)

# CMRC
FILE(GLOB_RECURSE web_resources RELATIVE ${PROJECT_SOURCE_DIR} src/contrib/webclient/dist/webclient/*)
cmrc_add_resource_library(DeepRTSResources ALIAS deeprts::rc
        WHENCE
        src/contrib
        ${web_resources}
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
        # Header-Only Libs
        include/DeepRTS/Constants.h
        include/DeepRTS/Config.h


        # Engine
        src/Game.cpp
        src/ResourceLoader.cpp

        # Player
        src/Player.cpp

        # Unit
        src/Unit.cpp
        src/UnitManager.cpp

        # Environment/Map
        src/Tile.cpp
        src/Tilemap.cpp
        src/Map.cpp

        # State
        src/state/BaseState.cpp
        src/state/Idle.cpp
        src/state/Walking.cpp
        src/state/Spawning.cpp
        src/state/StateManager.cpp
        src/state/Despawned.cpp
        src/state/Harvesting.cpp
        src/state/Building.cpp
        src/state/Combat.cpp
        src/state/Dead.cpp
        src/state/StateManager.cpp

        # Utilities
        include/DeepRTS/util/ColorConverter.hpp
        include/DeepRTS/util/Position.h
        src/util/Pathfinder.cpp
        include/DeepRTS/util/PriorityQueue.hpp

        include/DeepRTS/util/String.h

        src/PyGUI.cpp
        include/DeepRTS/PyGUI.h

        ${BINDING_SOURCES}
        ${WEBSERVER_SOURCES}
        )

set(DEEPRTS_PYBIND_SOURCES
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




if (NOT DEFINED PYTHON_BUILD)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/DeepRTS")
        find_package(Python ${PYTHON_REQUIRED_VERSION} REQUIRED COMPONENTS Interpreter Development)
endif ()


pybind11_add_module(Engine ${DEEPRTS_PYBIND_SOURCES})
target_link_libraries(Engine PRIVATE ${LIBRARIES})


# Dont create C++ Executable for Python builds (saves time)
if (NOT DEFINED PYTHON_BUILD)

        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/DeepRTS")  # Creates Engine in DeepRTS package (instead of root)
        add_executable(DeepRTSGame src/main.cpp ${SOURCE_FILES})
        target_include_directories(DeepRTSGame PRIVATE include/DeepRTS)
        target_compile_definitions(DeepRTSGame PRIVATE EMBEDDED=1)
        target_compile_definitions(DeepRTSGame PRIVATE WEBDEV=1)
        target_link_libraries(DeepRTSGame PRIVATE
                pybind11::embed
                ${DEEPRTS_LINK_TARGETS}
        )

        # make python source the build dir.
        execute_process(
                COMMAND ln -sf ${PROJECT_SOURCE_DIR}/DeepRTS ${CMAKE_BINARY_DIR}
        )

endif ()


