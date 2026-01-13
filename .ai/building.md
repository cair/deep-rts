# DeepRTS Build System

## Overview

DeepRTS uses CMake with VCPKG for C++ dependency management and setuptools with scikit-build for Python packaging.

## Build Methods

### 1. Install from Git (Recommended)

```bash
pip3 install git+https://github.com/cair/DeepRTS.git
```

### 2. Build from Source

```bash
git clone --recurse-submodules https://github.com/cair/deep-rts.git
cd deep-rts
pip3 install .
```

### 3. Development Install

```bash
pip3 install -e .
```

## Build Architecture

```
setup.py
    │
    ├── CMakeExtension("DeepRTS.Engine")
    │
    └── CMakeBuild
         │
         └──► CMakeLists.txt
              │
              ├── VCPKG bootstrap & install
              │    └── vcpkg_dependencies.txt
              │
              ├── Find packages
              │    ├── pybind11
              │    ├── OpenCV
              │    ├── spdlog
              │    ├── nlohmann_json
              │    ├── xtensor
              │    └── blend2d
              │
              ├── Compile sources
              │    ├── src/*.cpp
              │    └── bindings/*.cpp
              │
              ├── Embed assets (CMRC)
              │    └── DeepRTS/python/assets/*
              │
              └── Output: libdeeprts.so
```

## Dependencies

### C++ Dependencies (via VCPKG)

| Library | Purpose |
|---------|---------|
| **pybind11** | C++/Python bindings |
| **xtensor** | N-dimensional arrays |
| **xtensor-python** | NumPy interop for xtensor |
| **xsimd** | SIMD optimizations |
| **OpenCV** | Image processing, rendering |
| **blend2d** | 2D graphics engine |
| **spdlog** | Logging |
| **nlohmann-json** | JSON parsing |
| **effolkronium-random** | Random number generation |

### Python Dependencies

- NumPy
- OpenCV (cv2)
- Gym (optional, for RL integration)

## CMakeLists.txt Key Sections

### VCPKG Setup

```cmake
# Bootstrap VCPKG if needed
if(NOT EXISTS ${VCPKG_ROOT})
    execute_process(
        COMMAND git clone https://github.com/microsoft/vcpkg.git ${VCPKG_ROOT}
    )
    execute_process(
        COMMAND ${VCPKG_ROOT}/bootstrap-vcpkg.sh
    )
endif()

# Install dependencies
execute_process(
    COMMAND ${VCPKG_ROOT}/vcpkg install @${CMAKE_SOURCE_DIR}/vcpkg_dependencies.txt
)
```

### Compiler Flags

```cmake
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Release optimizations
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-Ofast -ffast-math)
endif()
```

### PyBind11 Module

```cmake
pybind11_add_module(libdeeprts
    ${BINDING_SOURCES}
    ${ENGINE_SOURCES}
)

target_link_libraries(libdeeprts PRIVATE
    ${OpenCV_LIBS}
    spdlog::spdlog
    blend2d::blend2d
    xtensor
    xtensor-python
)
```

### Asset Embedding (CMRC)

```cmake
cmrc_add_resource_library(deeprts_assets
    ALIAS deeprts::assets
    NAMESPACE deeprts
    ${ASSET_FILES}
)

target_link_libraries(libdeeprts PRIVATE deeprts::assets)
```

## vcpkg_dependencies.txt

```
pybind11
xtensor
xtensor-python
xsimd
opencv4
blend2d
spdlog
nlohmann-json
effolkronium-random
```

## Build Configuration Options

### CMake Variables

| Variable | Default | Description |
|----------|---------|-------------|
| `PYTHON_BUILD` | ON | Build Python module |
| `CMAKE_BUILD_TYPE` | Release | Build type |
| `ENABLE_LOCK` | ON | Prevent parallel CMake |

### Environment Variables

| Variable | Purpose |
|----------|---------|
| `VCPKG_ROOT` | Custom VCPKG location |
| `CMAKE_TOOLCHAIN_FILE` | VCPKG toolchain path |

## Platform Support

### Linux (manylinux)

- Built with manylinux2014 Docker images
- Uses system OpenCV when available
- VCPKG for other dependencies

### macOS

- Universal binaries (x86_64 + arm64)
- Homebrew dependencies supported
- VCPKG as fallback

### Windows

- MSVC compiler
- VCPKG for all dependencies
- Requires Visual Studio Build Tools

## CI/CD (cibuildwheel)

**File:** `pyproject.toml`

```toml
[tool.cibuildwheel]
build = "cp38-* cp39-* cp310-* cp311-*"
skip = "*-musllinux*"

[tool.cibuildwheel.linux]
before-all = "yum install -y opencv-devel"

[tool.cibuildwheel.macos]
before-all = "brew install opencv"
```

## Docker Build

**File:** `Dockerfile`

```dockerfile
FROM python:3.10-slim

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libopencv-dev

COPY . /app
WORKDIR /app

RUN pip install .
```

## Troubleshooting

### VCPKG Issues

```bash
# Clean VCPKG cache
rm -rf ~/.vcpkg

# Rebuild VCPKG
./vcpkg_install.sh
```

### OpenCV Not Found

```bash
# Linux
sudo apt install libopencv-dev

# macOS
brew install opencv

# Or let VCPKG handle it
```

### Build Cache

```bash
# Clear CMake cache
rm -rf build/
rm -rf _skbuild/

# Rebuild
pip install . --no-cache-dir
```

### Missing Assets

Assets are embedded at build time. If missing:

```bash
# Ensure assets exist
ls DeepRTS/python/assets/

# Rebuild with assets
pip install . --force-reinstall
```

## Performance Optimization

### Release Build

```bash
CMAKE_BUILD_TYPE=Release pip install .
```

### Enable ccache

```bash
export CMAKE_CXX_COMPILER_LAUNCHER=ccache
pip install .
```

### Parallel Compilation

```bash
CMAKE_BUILD_PARALLEL_LEVEL=8 pip install .
```

## File Locations

| File | Purpose |
|------|---------|
| `CMakeLists.txt` | Main CMake configuration |
| `setup.py` | Python package setup |
| `pyproject.toml` | Build system config |
| `vcpkg_dependencies.txt` | C++ dependencies list |
| `vcpkg_install.sh` | VCPKG installer script |
| `build.sh` | Build helper script |
| `Dockerfile` | Container build |
