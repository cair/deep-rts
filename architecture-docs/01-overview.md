# DeepRTS Architecture Overview

> **Analysis Date**: January 31, 2026
> **Version**: 3.0.0

## Executive Summary

DeepRTS is a high-performance Real-Time Strategy (RTS) game engine designed specifically for reinforcement learning research. The system achieves **6,000,000+ steps/second** without rendering (15,000% faster than StarCraft II) through a carefully designed hybrid architecture.

## System Purpose

- Provide an affordable, sustainable alternative to StarCraft II for RTS AI research
- Enable rapid experimentation with reinforcement learning algorithms
- Support both headless training and visual debugging modes
- Offer Gym-compatible interfaces for ML framework integration

## High-Level Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                     ML Frameworks Layer                         │
│              (Gym, PyTorch, Ray, TensorFlow)                   │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                   Python Wrapper Layer                          │
│   DeepRTS/python/game.py  │  DeepRTS/python/gui.py             │
│   - Game subclass          │  - Pygame visualization            │
│   - Gym action/obs spaces  │  - Sprite rendering                │
│   - Event handling         │  - Health bars, UI                 │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                   PyBind11 Bindings Layer                       │
│                      bindings/*.cpp                             │
│   - Type conversions       - NumPy array bridging               │
│   - Method exposure        - Callback trampolines               │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                    C++ Game Engine Core                         │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐             │
│  │    Game     │  │   Player    │  │    Unit     │             │
│  │  Controller │  │  Manager    │  │   System    │             │
│  └─────────────┘  └─────────────┘  └─────────────┘             │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐             │
│  │   Tilemap   │  │   State     │  │  Scenario   │             │
│  │   System    │  │  Machine    │  │   System    │             │
│  └─────────────┘  └─────────────┘  └─────────────┘             │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                    Rendering Backends                           │
│         Blend2D (C++)         │       Pygame (Python)           │
│    Fast native rendering      │   Rich visualization & UI       │
└─────────────────────────────────────────────────────────────────┘
```

## Key Design Decisions

### 1. Hybrid C++/Python Architecture
- **Rationale**: C++ for performance-critical game simulation, Python for ML ecosystem integration
- **Trade-off**: Build complexity in exchange for both speed and usability

### 2. Embedded Assets via CMRC
- **Rationale**: No external file dependencies at runtime
- **Implication**: Self-contained binary, easier deployment

### 3. Dual GUI Systems
- **Blend2D (C++)**: Fast native rendering for training with visualization
- **Pygame (Python)**: Rich UI for debugging and human observation

### 4. State Machine for Unit Behavior
- **Rationale**: Clean separation of unit states, easy to extend
- **Pattern**: Each unit maintains a state pointer, transitions managed by StateManager

## Core Metrics

| Metric | Value |
|--------|-------|
| **Steps/sec (no render)** | 6,000,000+ |
| **Steps/sec (with render)** | 2,000,000+ |
| **Max Players** | 8 |
| **Max Units** | 1000 |
| **Action Space** | 16 discrete actions |
| **Map Sizes** | 10x10 to 31x31 |

## Technology Stack Summary

| Layer | Technologies |
|-------|--------------|
| **Language** | C++17, Python 3.8+ |
| **Build** | CMake, setuptools, VCPKG |
| **Bindings** | PyBind11 |
| **Arrays** | xtensor, NumPy |
| **Graphics** | Blend2D, OpenCV, Pygame |
| **JSON** | nlohmann_json |
| **Logging** | spdlog |

## Document Index

| Document | Description |
|----------|-------------|
| [02-technology-inventory.md](02-technology-inventory.md) | Complete technology stack with versions |
| [03-component-architecture.md](03-component-architecture.md) | Detailed component relationships |
| [04-interfaces.md](04-interfaces.md) | API contracts and boundaries |
| [05-data-flow.md](05-data-flow.md) | Game loop and data flow diagrams |
| [06-state-machine.md](06-state-machine.md) | Unit state machine specification |
| [07-scenario-system.md](07-scenario-system.md) | Training scenarios and criteria |
| [08-build-system.md](08-build-system.md) | Build pipeline and dependencies |
| [09-documentation-audit.md](09-documentation-audit.md) | Cross-reference with existing docs |
