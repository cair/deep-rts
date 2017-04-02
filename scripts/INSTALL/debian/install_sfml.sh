# Check if already installed

if [ -d "/usr/local/include/SFML/" ]; then
  echo "SFML already installed!"
  return 1
fi

# Delete existing installations
rm -rf /tmp/sfml/

# Create new directory for compilation
mkdir /tmp/sfml/

# Enter temp directory
cd /tmp/sfml/

# Download SFML
git clone https://github.com/SFML/SFML.git .

# Create Makefiles
cmake .

# Compile SFML
make -j 8

# Install SFML
make install