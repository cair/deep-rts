
if [ -d "/usr/local/include/gpuarray/" ]; then
  echo "GPUArray already installed!"
  return 1
fi

# Delete temp directory
rm -rf /tmp/libgpuarray

# Create temp directory
mkdir /tmp/libgpuarray

# Enter temp directory
cd /tmp/libgpuarray

# Clone directory
git clone https://github.com/Theano/libgpuarray.git .

# Create build directory
mkdir build

# Enter build directory
cd build

# Create makefiles
cmake .. -DCMAKE_BUILD_TYPE=Release

# Compile
make -j 8

# Install
make install

# Go back to main dir
cd ..

# Install for python
pip3 install cython
python3 setup.py build
python3 setup.py install

