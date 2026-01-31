# Technology Inventory

> Complete inventory of languages, frameworks, libraries, and tools.

## Languages

| Language | Version | Purpose | Location |
|----------|---------|---------|----------|
| **C++17** | Standard | Core game engine, performance-critical code | `src/`, `include/` |
| **Python** | 3.8+ | ML integration, GUI, wrapper layer | `DeepRTS/python/` |
| **CMake** | 3.17+ | Build system configuration | `CMakeLists.txt` |

## Core Dependencies (VCPKG)

Managed via [vcpkg_dependencies.txt](../vcpkg_dependencies.txt):

| Dependency | Purpose | Used In |
|------------|---------|---------|
| **pybind11** | C++/Python bindings | `bindings/*.cpp` |
| **xtensor[xsimd]** | N-dimensional arrays with SIMD | `Game.h`, state representation |
| **spdlog** | Fast logging | Throughout C++ code |
| **nlohmann-json** | JSON parsing | Map loading, config |
| **opencv** | Image processing, Mat type | Render output |
| **blend2d** | 2D vector graphics | `Blend2DGUI.cpp` |
| **effolkronium-random** | Random number generation | Action sampling |

## Python Dependencies

From [requirements.txt](../requirements.txt) and runtime imports:

| Package | Purpose | Used In |
|---------|---------|---------|
| **numpy** | Array operations | State representation |
| **pygame** | Visualization, UI | `DeepRTS/python/gui.py` |
| **opencv-python** | Image display | Demo scripts |
| **gym** | RL environment interface | `game.py` action/obs spaces |

## Build Tools

| Tool | Version | Purpose |
|------|---------|---------|
| **CMake** | 3.17+ | Cross-platform build configuration |
| **VCPKG** | 2023.04.15 | C++ package management |
| **setuptools** | 41+ | Python package building |
| **ninja** | Any | Fast parallel builds |
| **ccache** | Optional | Compilation caching |

## Development Dependencies

| Tool | Purpose |
|------|---------|
| **Doxygen** | API documentation generation |
| **clang++/g++** | C++ compiler |
| **pybind11[global]** | Python bindings (build-time) |

## Asset Pipeline

| Type | Format | Location |
|------|--------|----------|
| **Maps** | JSON | `DeepRTS/python/assets/maps/*.json` |
| **Textures** | PNG | `DeepRTS/python/assets/textures/` |
| **Tile Properties** | JSON | `DeepRTS/python/assets/tile_properties.json` |
| **Audio** | OGG | `DeepRTS/python/assets/audio/` |
| **Fonts** | TTF | `DeepRTS/python/assets/fonts/` |

## Embedded Resources

Assets are compiled into the binary using **CMRC** (CMake Resource Compiler):

```cmake
# From CMakeLists.txt
FILE(GLOB_RECURSE deeprts_assets RELATIVE ${PROJECT_SOURCE_DIR} DeepRTS/python/assets/*)
cmrc_add_resource_library(DeepRTSAssets ALIAS deeprts::assets
    WHENCE DeepRTS/python
    ${deeprts_assets}
)
```

## Version Requirements Matrix

| Component | Minimum | Recommended | Notes |
|-----------|---------|-------------|-------|
| Python | 3.8 | 3.10+ | NumPy compatibility |
| CMake | 3.17 | 3.25+ | Modern features |
| C++ Standard | C++17 | C++17 | Required, not optional |
| NumPy | Any | Latest | xtensor-python compatibility |

## Platform Support

| Platform | Architecture | Status |
|----------|--------------|--------|
| Linux | x86_64 | Primary target |
| macOS | x86_64, arm64 | Supported |
| Windows | x64 | Supported (MSVC) |

## CI/CD Tools

From [.travis.yml](../.travis.yml) and [pyproject.toml](../pyproject.toml):

| Tool | Purpose |
|------|---------|
| **Travis CI** | Continuous integration |
| **cibuildwheel** | Cross-platform wheel building |
| **auditwheel/delocate** | Wheel repair for Linux/macOS |
