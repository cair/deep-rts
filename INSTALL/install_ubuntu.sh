#!/bin/bash
if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

chmod +x -R ../src/flatbuffers/

# BUILD TOOLS
# sfml-window
apt-get install libx11-dev -y
apt-get install libxcb1-dev -y
apt-get install libx11-xcb-dev -y
apt-get install libxcb-randr0-dev -y
apt-get install libxcb-image0-dev -y
apt-get install libgl1-mesa-dev -y
apt-get install libudev-dev -y

# sfml-graphics
apt-get install libfreetype6-dev -y
apt-get install libjpeg-dev -y

# sfml-audio
apt-get install libopenal-dev -y
apt-get install libflac-dev -y
apt-get install libvorbis-dev -y


# SFML
rm -rf /tmp/sfml/
cd /tmp/
mkdir sfml
cd sfml
wget -O sfml.zip https://www.sfml-dev.org/files/SFML-2.4.2-sources.zip
unzip -j sfml.zip
cmake .
make
make install


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