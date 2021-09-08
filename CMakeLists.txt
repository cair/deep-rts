cmake_minimum_required(VERSION 3.17)
project(DeepRTS VERSION 1.1.0 LANGUAGES CXX)

# Lock this section so that parallel CMake runs won't clash on checkout in the same directory
file(LOCK ${CMAKE_SOURCE_DIR} DIRECTORY GUARD FILE)

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


if (WIN32)
        set(OPTIMIZATIONS_RELEASE /Ox)
        set(OPTIMIZATIONS_DEBUG /O0)
else ()
        set(OPTIMIZATIONS_RELEASE -Ofast -march=native -ffast-math -Wall)
        set(OPTIMIZATIONS_DEBUG -Wall -O0)
endif ()


if (CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(${OPTIMIZATIONS_RELEASE})
else()
        add_compile_options(${OPTIMIZATIONS_DEBUG})
        add_compile_definitions(DEBUG=1)
        add_compile_options(-gdwarf-4)  # This allows per to debug with llvm
endif()


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
        Blend2d
        #opencv  # todo remove later
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
find_package(OpenCV CONFIG REQUIRED )
find_package(OpenSSL REQUIRED)
find_package(Drogon CONFIG REQUIRED)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(effolkronium_random CONFIG REQUIRED)
find_package(xtensor CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

find_library(blend2d blend2d REQUIRED)
find_path(blend2d_INCLUDES blend2d.h)

find_package(Doxygen)
if (DOXYGEN_FOUND AND NOT PYTHON_BUILD)
        # set input and output files
        set(DOXYGEN_IN Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build started")

        # note the option ALL which allows to build the docs together with the application
        add_custom_target( doc_doxygen ALL
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating API documentation with Doxygen"
                VERBATIM )
else ()
        message("Doxygen need to be installed to generate the doxygen documentation")
endif ()


set(DEEPRTS_LINK_TARGETS
        spdlog::spdlog
        DeepRTSResources
        DeepRTSAssets
        effolkronium_random
        Python3::Python
        xtensor::optimize
        xtensor::use_xsimd
        Drogon::Drogon
        rt  # asmjit
        ${blend2d}
        opencv_core  # todo remove
        opencv_highgui  # todo remove
)





set(DEEPRTS_INCLUDES
        ${blend2d_INCLUDES}

)



# CMRC
FILE(GLOB_RECURSE web_resources RELATIVE ${PROJECT_SOURCE_DIR} src/contrib/webclient/dist/webclient/*)
cmrc_add_resource_library(DeepRTSResources ALIAS deeprts::rc
        WHENCE
        src/contrib
        ${web_resources}
        )


#####################################
##
## CMRC - Assets for game
##
######################################
FILE(GLOB_RECURSE deeprts_assets RELATIVE ${PROJECT_SOURCE_DIR} DeepRTS/python/assets/*)
cmrc_add_resource_library(DeepRTSAssets ALIAS deeprts::assets
        WHENCE
        DeepRTS/python
        ${deeprts_assets}
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
        bindings/utilities/ndarray_converter.cpp bindings/Scenarios.cpp bindings/trampolines/PyScenarioCriteria.h)

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
        include/DeepRTS/gui/PyGUI.h

        ${BINDING_SOURCES}
        ${WEBSERVER_SOURCES}
        include/DeepRTS/gui/Blend2DGUI.h include/DeepRTS/unit/Footman.h include/DeepRTS/gui/BaseGUI.h src/gui/BaseGUI.cpp src/gui/Blend2DGUI.cpp include/DeepRTS/gui/Blend2DSprites.h src/scenario/Scenario.cpp include/DeepRTS/scenario/Scenario.h src/scenario/criteria/ScenarioCriteria.cpp include/DeepRTS/scenario/criterias/ScenarioCriteria.h src/scenario/criteria/GoldCollect.cpp include/DeepRTS/scenario/criterias/GoldCollect.h src/scenario/criteria/LumberCollect.cpp include/DeepRTS/scenario/criterias/LumberCollect.h src/scenario/criteria/StoneCollect.cpp include/DeepRTS/scenario/criterias/StoneCollect.h src/scenario/criteria/FoodConsumption.cpp include/DeepRTS/scenario/criterias/FoodConsumption.h src/scenario/criteria/FoodCount.cpp include/DeepRTS/scenario/criterias/FoodCount.h src/scenario/criteria/DamageDone.cpp include/DeepRTS/scenario/criterias/DamageDone.h src/scenario/criteria/DamageTaken.cpp include/DeepRTS/scenario/criterias/DamageTaken.h src/scenario/criteria/UnitsCreated.cpp include/DeepRTS/scenario/criterias/UnitsCreated.h src/scenario/criteria/NumUnitTypeCreated.cpp include/DeepRTS/scenario/criterias/NumUnitTypeCreated.h src/scenario/scenarios/GoldCollectFifteen.cpp include/DeepRTS/scenario/scenarios/GoldCollectFifteen.h src/scenario/scenarios/GeneralAIOneVersusOne.cpp include/DeepRTS/scenario/scenarios/GeneralAIOneVersusOne.h src/scenario/criteria/ResourceIncrement.cpp include/DeepRTS/scenario/criterias/ResourceIncrement.h src/scenario/criteria/UnitIncrement.cpp include/DeepRTS/scenario/criterias/UnitIncrement.h src/scenario/criteria/DamageDoneIncrement.cpp include/DeepRTS/scenario/criterias/DamageDoneIncrement.h src/scenario/criteria/DamageTakenIncrement.cpp include/DeepRTS/scenario/criterias/DamageTakenIncrement.h)




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

endif ()


if(PYTHON_BUILD)
        CPMADDPackage(
                NAME xtensor_py
                GIT_REPOSITORY "https://github.com/xtensor-stack/xtensor-python"
                GIT_TAG 0.25.3
        )



find_package(Python ${PYTHON_REQUIRED_VERSION} REQUIRED COMPONENTS Interpreter Development NumPy)
set_property(TARGET DeepRTSResources PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET DeepRTSAssets PROPERTY POSITION_INDEPENDENT_CODE ON)
pybind11_add_module(Engine ${DEEPRTS_PYBIND_SOURCES})
target_link_libraries(Engine INTERFACE xtensor-python PRIVATE                Python::NumPy ${DEEPRTS_LINK_TARGETS})
target_include_directories(Engine PRIVATE
        include/DeepRTS
        ${DEEPRTS_INCLUDES}
        ${xtensor_py_SOURCE_DIR}/include
        )
else()
        find_package(Python ${PYTHON_REQUIRED_VERSION} REQUIRED COMPONENTS Interpreter Development)
endif()
# Dont create C++ Executable for Python builds (saves time)
if (NOT DEFINED PYTHON_BUILD)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/DeepRTS")  # Creates Engine in DeepRTS package (instead of root)
        add_executable(DeepRTSGame src/main.cpp ${SOURCE_FILES})
        target_include_directories(DeepRTSGame PRIVATE include/DeepRTS ${DEEPRTS_INCLUDES})
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


