#!/usr/bin/env bash
# Check if already installed

#if [ -d "/usr/local/include/SFML/" ]; then
#  echo "SFML already installed!"
#  return 1
#fi

# Install ZLIB
apt-get update
apt-get install zlibc libuv-dev libssl-dev -y


# Delete existing installations
rm -rf /tmp/uwebsockets/

# Create new directory for compilation
mkdir /tmp/uwebsockets/

# Enter temp directory
cd /tmp/uwebsockets/

# Download Uwebsockets
git clone https://github.com/uWebSockets/uWebSockets.git .

# Compile uWebsockets
make -j 8

# Install uWebsockets
make install