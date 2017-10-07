#!/bin/bash
if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

CORES=$(cat /proc/cpuinfo | awk '/^processor/{print $3}' | tail -1)
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
chmod +x -R ../c++/src/flatbuffers/




# Packages
yum install mesa-libGL-devel xcb-util-image-devel libudev-devel libjpeg-devel openal-devel libvorbis-devel flac-devel -y


# SFML
rm -rf /tmp/sfml/
cd /tmp/
mkdir sfml
cd sfml
wget -O sfml.zip http://mirror2.sfml-dev.org/files/SFML-2.4.2-sources.zip
unzip -o sfml.zip
cd SFML-2.4.2
cmake .
make -j $CORES
make install


# FLATBUFFERS
if which /usr/local/bin/flatc >/dev/null; then
    echo "Flatbuffers already installed, ignoring..."
else
    rm -rf /tmp/flatbuffers-install
    mkdir -p /tmp/flatbuffers-install
    cd /tmp/flatbuffers-install
    git clone https://github.com/google/flatbuffers.git
    cd flatbuffers
    cmake -G "Unix Makefiles"
    make -j $CORES
    ./flattests
    make install
    cd
fi


# Generate Flatbuffer headers
cd $DIR
ls -la
cd ../c++/src/flatbuffers/ScoreLog
PATH=/usr/local/bin:$PATH
sh create_buffer.sh


# PYTHON 3
yum install python3 python3-pip -y
pip3 install keras theano flatbuffers

echo "You are now ready to compile the project! :)"
