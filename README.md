# WarC2Sim++
A Warcraft 2 Simulator for (Deep) Reinforment Learning in C++!

## Dependencies and use cases
* SFML - used for drawing graphics in presentation mode, not used when deactivated in configs (except for a few calls like sf::Vector)
* Protobuf - Current Serialization library
* Flatbuffers - Future Serialization library
* Python 2.7.x/3.6.x - For use with scientific Machine learning connected to the game
* Optional:
    * ZeroMQ is a hyper-ultra-mega lightweight and fast message queue which is used to transfer messages between runtimes like Python. This is specifically useful when you dont want to use C++ for the AI part.

### Installing (Using vcpkg)
1. Install Anaconda x64 bit
2. conda create --prefix=C:\Python27 python=2.7
* Install vcpkg
* Follow integration instructions
```bash
vcpkg.exe install sfml:x64-windows
vcpkg.exe install protobuf:x64-windows
vcpkg.exe install flatbuffers:x64-windows
```
* It is recommended that you add `vcpkg\installed\x64-windows\tools` to PATH as you will be needing `flatc.exe`
