cmake_minimum_required(VERSION 3.17)
project(DeepRTS VERSION 1.1.0 LANGUAGES CXX)


set(BUILD_DOCS "OFF" CACHE STRING "Helpstring")
SET(ENABLE_LOCK "ON" CACHE STRING "If locking of cmake setup should be enabled. this should be disabled for third party builds.")
set(PYBIND11_FINDPYTHON "OFF")
# Lock this section so that parallel CMake runs won't clash on checkout in the same directory
if(ENABLE_LOCK)
        file(LOCK ${CMAKE_SOURCE_DIR} DIRECTORY GUARD FILE)
endif()
# Includes
include(cmake/CMakeRC.cmake)
include(cmake/CPM.cmake)
include(cmake/pmm.cmake)

#############################################################################################
#####
##### C++ Compiler Setup
#####
#############################################################################################
set(CMAKE_CXX_STANDARD 17)
set(PYBIND11_CPP_STANDARD /std:c++17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)



if (LIBRARY_TYPE STREQUAL "SHARED")
        set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif ()

find_program(CCACHE_FOUND ccache)
    if(CCACHE_FOUND)
        set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_FOUND})
    else()
        message(WARNING "ccache not found, compilation will not be cached.")
endif()




if (WIN32)
        set(OPTIMIZATIONS_RELEASE /Ox)
        set(OPTIMIZATIONS_DEBUG /O0)
else ()
        set(OPTIMIZATIONS_RELEASE -Ofast -ffast-math -Wall) # -march=native
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
elseif (LINUX)
        set(VCPKG_TARGET_TRIPLET x64-linux)
elseif(APPLE)
        # Get the macOS version
        execute_process(
            COMMAND sw_vers -productVersion
            OUTPUT_VARIABLE MACOS_VERSION
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        # Extract the major and minor versions
        string(REGEX REPLACE "^([0-9]+\\.[0-9]+).*" "\\1" MACOS_VERSION ${MACOS_VERSION})
        # Set the deployment target
        set(CMAKE_OSX_DEPLOYMENT_TARGET ${MACOS_VERSION} CACHE STRING "Minimum OS X deployment version")
    endif()
    

#############################################################################################
#####
##### VCPKG - Dependencies
#####
#############################################################################################
# Read the contents of the file into a list

if(NOT (CMAKE_TOOLCHAIN_FILE MATCHES "vcpkg"))
        file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_dependencies.txt" DEEPRTS_DEPENDENCIES)
        # VCPKG - Install Dependencies
        pmm(
                VERBOSE
                DEBUG
                VCPKG
                REVISION 2023.04.15
                REQUIRES ${DEEPRTS_DEPENDENCIES}
                TRIPLET ${VCPKG_TARGET_TRIPLET}
        )
endif()



#############################################################################################
#####
##### CMAKE - Find Packages
#####
#############################################################################################

find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
set(PYTHON_EXECUTABLE ${Python_EXECUTABLE})  # xtensor has some troubles. fix

find_package(Python COMPONENTS NumPy)
if(NOT Python_NumPy_FOUND)
        execute_process(
                COMMAND bash -c "curl -L https://bootstrap.pypa.io/get-pip.py | ${Python_EXECUTABLE} && ${Python_EXECUTABLE} -m pip install numpy"
        )
        find_package(Python REQUIRED COMPONENTS NumPy)
endif()

find_package(spdlog CONFIG REQUIRED)
find_package(OpenCV CONFIG REQUIRED )
find_package(effolkronium_random CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(xtensor CONFIG REQUIRED)
find_library(blend2d blend2d REQUIRED)
find_path(blend2d_INCLUDES blend2d.h)



if (NOT TARGET xtensor-python)
        CPMADDPackage(
                NAME xtensor_py
                GIT_REPOSITORY "https://github.com/xtensor-stack/xtensor-python"
                GIT_TAG 0.25.3
        )
endif()
set(DEEPRTS_LINK_TARGETS
        spdlog::spdlog
        DeepRTSAssets
        effolkronium_random

        xtensor::optimize
        xtensor::use_xsimd
        ${blend2d}
        opencv_core  # todo remove
        opencv_highgui  # todo remove
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

set_property(TARGET DeepRTSAssets PROPERTY POSITION_INDEPENDENT_CODE ON)



file(GLOB_RECURSE DEEPRTS_SOURCE_FILES src/*.cpp)


set(DEEPRTS_PYBIND_SOURCES
        bindings/Random.cpp
        bindings/utilities/ndarray_converter.cpp
        bindings/Scenarios.cpp
        bindings/trampolines/PyScenarioCriteria.h
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

set(DEEPRTS_INCLUDES
        ${blend2d_INCLUDES}
        include/DeepRTS
        ${xtensor_py_SOURCE_DIR}/include
)


if (NOT DEFINED PYTHON_BUILD)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/DeepRTS")

endif ()


if(PYTHON_BUILD)
        pybind11_add_module(libdeeprts ${DEEPRTS_PYBIND_SOURCES} ${DEEPRTS_SOURCE_FILES})
        target_link_libraries(libdeeprts
                INTERFACE
                xtensor-python
                PRIVATE
                Python::NumPy ${DEEPRTS_LINK_TARGETS}
        )

        target_include_directories(libdeeprts
                PRIVATE
                ${DEEPRTS_INCLUDES}
        )
endif()

# Dont create C++ Executable for Python builds (saves time)
if (NOT DEFINED PYTHON_BUILD)
        add_library(DeepRTSLib STATIC ${DEEPRTS_SOURCE_FILES})
        target_include_directories(
                DeepRTSLib
                PUBLIC
                include/
                include/DeepRTS
                ${DEEPRTS_INCLUDES}
        )
        target_compile_definitions(DeepRTSLib
                PUBLIC
                EMBEDDED=1
                )
        target_link_libraries(DeepRTSLib
                PUBLIC
                xtensor-python
                pybind11::embed
                ${DEEPRTS_LINK_TARGETS}
                )
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/DeepRTS")  # Creates Engine in DeepRTS package (instead of root)
        add_executable(DeepRTSGame src/main.cpp)

        #target_include_directories(DeepRTSGame
        #        PRIVATE
        #        DeepRTSLib
        #        )

        target_link_libraries(DeepRTSGame
                PRIVATE
                DeepRTSLib
        )


        # make python source the build dir.
        execute_process(
                COMMAND ln -sf ${PROJECT_SOURCE_DIR}/DeepRTS ${CMAKE_BINARY_DIR}
        )
endif()



#############################################################################################
#####
##### Documentation
#####
#############################################################################################
find_package(Doxygen)

if (DOXYGEN_FOUND AND NOT PYTHON_BUILD AND BUILD_DOCS)
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
        message(WARNING "Doxygen need to be installed to generate the doxygen documentation")
endif ()


