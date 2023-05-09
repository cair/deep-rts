#!/bin/bash

# Dependencies required for building Vcpkg
DEBIAN_PACKAGES="build-essential tar curl zip unzip git ninja"
FEDORA_PACKAGES="gcc gcc-c++ make curl tar unzip zip git ninja"
CENTOS_PACKAGES="gcc gcc-c++ make curl tar unzip zip git ninja"
ARCH_PACKAGES="base-devel curl tar unzip zip git ninja"
MACOS_PACKAGES="cmake curl unzip tar git ninja"

# Function to install packages on Debian
function install_packages_debian() {
    sudo apt-get update
    sudo apt-get install -y ${DEBIAN_PACKAGES}
}

# Function to install packages on Fedora
function install_packages_fedora() {
    sudo dnf install -y ${FEDORA_PACKAGES}
}

# Function to install packages on CentOS
function install_packages_centos() {
    sudo yum install -y ${CENTOS_PACKAGES}
}

# Function to install packages on Arch Linux
function install_packages_arch() {
    sudo pacman -Syu --needed ${ARCH_PACKAGES}
}

# Function to install packages on MacOS
function install_packages_macos() {
    # Verify if 'brew' is installed
    if ! command -v brew &> /dev/null
    then
        echo "Brew is not installed. Install it first and run the script again."
        exit 1
    fi

    # If 'brew' is installed, use it to install the packages
    for PACKAGE in ${MACOS_PACKAGES}
    do
        brew install ${PACKAGE}
    done
}

# Identify the Linux distribution
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    if type apt-get > /dev/null 2>&1; then
        install_packages_debian
    elif type dnf > /dev/null 2>&1; then
        install_packages_fedora
    elif type yum > /dev/null 2>&1; then
        install_packages_centos
    elif type pacman > /dev/null 2>&1; then
        install_packages_arch
    else
        echo "Unknown package manager. Please install the dependencies manually."
    fi
elif [[ "$OSTYPE" == "darwin"* ]]; then
    install_packages_macos
elif [[ "$OSTYPE" == "cygwin" ]]; then
    echo "Cygwin detected. Please install the required dependencies manually."
elif [[ "$OSTYPE" == "msys" ]]; then
    echo "Git Bash detected. Please install the required dependencies manually."
else
    echo "Unknown operating system. Please install the required dependencies manually."
fi

# Clone or update Vcpkg
if [ ! -d "vcpkg/vcpkg" ]; then
  git clone https://github.com/Microsoft/vcpkg.git vcpkg/vcpkg
else
  cd vcpkg && git pull && cd ..
fi


# Build and install Vcpkg
./vcpkg/vcpkg/bootstrap-vcpkg.sh
./vcpkg/vcpkg/vcpkg integrate install


# Install libraries from txt file
while read library; do
  ./vcpkg/vcpkg/vcpkg install $library
done < vcpkg/vcpkg_dependencies.txt
