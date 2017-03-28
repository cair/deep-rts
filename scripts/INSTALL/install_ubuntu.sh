#!/bin/bash
if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
chmod +x -R ../c++/src/flatbuffers/

# Packages
apt-get install libx11-dev libxcb1-dev libx11-xcb-dev libxcb-randr0-dev libxcb-image0-dev libgl1-mesa-dev libudev-dev libfreetype6-dev libjpeg-dev libopenal-dev libflac-dev libvorbis-dev -y

# SFML
rm -rf /tmp/sfml/
cd /tmp/
mkdir sfml
cd sfml
wget -O sfml.zip http://mirror2.sfml-dev.org/files/SFML-2.4.2-sources.zip
unzip -o sfml.zip
cd SFML-2.4.2
cmake .
make -j 8
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
    make -j 8
    ./flattests
    make install
    cd
fi

# Generate Flatbuffer headers
cd $DIR
ls -la
cd ../c++/src/flatbuffers/ScoreLog
sh create_buffer.sh


# PYTHON 3
apt-get install python3 python3-pip -y
pip3 install keras theano flatbuffers

echo "You are now ready to compile the project! :)"