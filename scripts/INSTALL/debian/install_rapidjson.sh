if [ -d "/usr/local/include/rapidjson/" ]; then
  echo "RapidJSON already installed!"
  return 1
fi

# Delete temp directory
rm -rf /tmp/rapidjson

# Create temp directory
mkdir /tmp/rapidjson

# Enter temp directory
cd /tmp/rapidjson

# Clone directory
git clone https://github.com/miloyip/rapidjson.git .

# Create build directory
mkdir build

# Enter build directory
cd build

# Create makefiles
cmake ..

# Compile
make -j 8

# Install
make install

