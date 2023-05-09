#!/bin/bash

# Get the absolute path of the current directory
CURRENT_DIR=$(pwd)

# Build directory = Current directory + /build
BUILD_DIR=build

# Create build dir
mkdir -p $BUILD_DIR

# VC pkg dir = Current directory + /vcpkg
VCPKG_CONFIG_DIR=$(realpath "${CURRENT_DIR}/vcpkg")
VCPKG_DIR=$(realpath "${VCPKG_CONFIG_DIR}/vcpkg")
TOOLCHAIN_FILE=$VCPKG_DIR/scripts/buildsystems/vcpkg.cmake

MACOS_VERSION=$(sw_vers -productVersion | cut -d'.' -f1,2)
export MACOSX_DEPLOYMENT_TARGET=$MACOS_VERSION

# Install dependencies
$VCPKG_CONFIG_DIR/vcpkg_install.sh



# CMake
cmake \
-GNinja \
-B $BUILD_DIR \
-S . \
-DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE \
-DCMAKE_BUILD_TYPE=Release

# CMake
cmake \
--build $BUILD_DIR