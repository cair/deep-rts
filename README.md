# WarC2Sim++
A Warcraft 2 Simulator for (Deep) Reinforment Learning in C++!

## Dependencies and use cases
* SFML are used for drawing graphics in presentation mode, not used when deactivated in configs (except for a few calls like sf::Vector)
* ZeroMQ is a hyper-ultra-mega lightweight and fast message queue which is used to transfer messages between runtimes like Python. This is specifically useful when you dont want to use C++ for the AI part.

## Compilation
In order to get things running you must have the following dependencies:
* SFML v2.x
* ZeroMQ 4.x
We will be using MSYS2 with Mingw64.

### Installing (Using vcpkg)
1. Install Anaconda x64 bit
2. conda create --prefix=C:\Python27 python=2.7

* Install vcpkg
* Follow integration instructions
```bash
vcpkg.exe install sfml:x64-windows
vcpkg.exe install protobuf:x64-windows
vcpkg.exe install uwebsockets:x64-windows
```

### Installing (Fast Mode)
* Install MSYS2
* run pacman -Sy
* Install the following packages:
```bash
 pacman -Sy
 pacman -S git
 pacman -S make
 pacman -S mingw-w64-x86_64-gcc
 pacman -S mingw-w64-x86_64-gdb # ability to debug is always useful
 pacman -S mingw-w64-x86_64-cmake
 pacman -S mingw-w64-x86_64-pkg-config
 pacman -S mingw-w64-x86_64-pkg-sfml
 pacman -S mingw-w64-x86_64-pkg-zeromq
 pacman -S mingw-w64-x86_64-toolchain
 pacman -S mingw-w64-x86_64-pkg-config
 pacman -S mingw-w64-x86_64-protobuf
```
* **Remember to add mingw bin to path!**
* Everything should work now as long as you are using gcc through msys2


### Instaling MSYS2
* Go to http://www.msys2.org/ and download msys2-x86_64-xxxxx.exe (64-bit)
* Follow the detailed guide on their front-page
* Install following packages using pacman (packman -S <package>):
    * make 
    * automake
    * gcc
    * libtool
    * autoconf
* Now run gcc -v and confirm that gcc is installed (gcc version 6.3.0 (x86_64-posix-seh-rev1, Built by MinGW-W64 project))
* **Remember to put mingw64 to PATH in Windows environments**

### Compiling ZeroMQ
* Download latest stable POSIX source file package from http://zeromq.org/area:download
* Extract files to /build-env/src/  (so that you have /build-env/src/zeromq)
```bash
cd /build-env/src/zeromq/
./autogen.sh # Takes a few minutes
./configure --prefix=/build-env/bin/zeromq
make
make install
```
* ZeroMQ is now ready!

### Compiling SFML
* Download latest stable source from http://www.sfml-dev.org/download.php
* Rest is TODO 
