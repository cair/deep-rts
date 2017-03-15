# DeepRTS  [![Build Status](https://travis-ci.org/UIA-CAIR/DeepRTS.svg)](https://travis-ci.org/UIA-CAIR/DeepRTS) [![Documentation](https://img.shields.io/badge/docs-readme-blue.svg)](https://github.com/UIA-CAIR/DeepRTS/blob/c%2B%2B/docs/README.md) [![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/UIA-CAIR/DeepRTS/c%2B%2B/LICENCE.MIT)

A Real-Time Strategy Simulator for (Deep) Reinforment Learning in C++!

## Dependencies and use cases
* SFML - used for drawing graphics in presentation mode, not used when deactivated in configs (except for a few calls like sf::Vector)
* Flatbuffers - Future Serialization library
* Python 3.6.x - For use with scientific Machine learning connected to the game
* Optional:
    * ZeroMQ is a hyper-ultra-mega lightweight and fast message queue which is used to transfer messages between runtimes like Python. This is specifically useful when you dont want to use C++ for the AI part.

