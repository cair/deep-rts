#!/bin/bash
if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

chmod +x -R ../src/flatbuffers/

# SFML
apt-get install libsfml-dev git -y

# BUILD TOOLS
apt-get install build-essentials -y
apt-get install cmake -y


# FLATBUFFERS
if which flatc >/dev/null; then
    echo "Flatbuffers already installed, ignoring..."
else
    rm -rf /tmp/flatbuffers-install
    mkdir -p /tmp/flatbuffers-install
    cd /tmp/flatbuffers-install
    git clone https://github.com/google/flatbuffers.git
    cd flatbuffers
    cmake -G "Unix Makefiles"
    make
    ./flattests
    make install
    cd
fi
# PYTHON 3
apt-get install python3 python3-pip -y
pip3 install keras theano flatbuffers

echo "You are now ready to compile the project! :)"