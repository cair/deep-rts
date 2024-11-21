#!/bin/bash

vcpkg_target_dir=$1

# For Fedora
FEDORA_PACKAGES="gcc gcc-c++ make curl tar unzip zip git ninja-build opencv opencv-devel perl dbus-devel libXi-devel libXtst-devel"

# For Debian-based systems like Ubuntu
DEBIAN_PACKAGES="build-essential tar curl zip unzip git ninja-build libopencv-dev perl libdbus-1-dev libxi-dev libxtst-dev"

# For CentOS
CENTOS_PACKAGES="gcc gcc-c++ make curl tar unzip zip git ninja-build opencv opencv-devel perl dbus-devel libXi-devel libXtst-devel"

# For Arch Linux
ARCH_PACKAGES="base-devel curl tar unzip zip git ninja opencv perl dbus libxi libxtst"

# For MacOS (uses Homebrew)
MACOS_PACKAGES="cmake curl unzip tar git ninja opencv perl dbus libxi libxtst"

# For Alpine Linux (uses apk)
APK_PACKAGES="gcc g++ make curl tar unzip zip git ninja perl dbus-dev libxi-dev libxtst-dev"


# Function to install packages on Debian
function install_packages_debian() {
    apt-get update
    apt-get install -y ${DEBIAN_PACKAGES}
}

# Function to install packages on Fedora
function install_packages_fedora() {
    dnf install -y ${FEDORA_PACKAGES}
}

# Function to install packages on CentOS
function install_packages_centos() {
    yum install -y ${CENTOS_PACKAGES}
}

# Function to install packages on Arch Linux
function install_packages_arch() {
    pacman -Syu --needed ${ARCH_PACKAGES}
}

function install_packages_musl() {
     apk add --no-cache ${APK_PACKAGES}
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

echo $OSTYPE

# Identify the Linux distribution
if [[ "$OSTYPE" == "linux-gnu"* ]] || [[ "$OSTYPE" == "linux-musl"* ]] || [[ "$OSTYPE" == "linux"* ]]; then


    if type apt-get > /dev/null 2>&1; then
        install_packages_debian
    elif type dnf > /dev/null 2>&1; then
        install_packages_fedora
    elif type yum > /dev/null 2>&1; then
        install_packages_centos
    elif type pacman > /dev/null 2>&1; then
        install_packages_arch
    elif type apk > /dev/null 2>&1; then
        install_packages_musl
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
if [ ! -d "$vcpkg_target_dir/vcpkg" ]; then

  cd $vcpkg_target_dir && git clone https://github.com/Microsoft/vcpkg.git
else
  cd $vcpkg_target_dir/vcpkg && git pull && cd ..
fi


# Build and install Vcpkg
$vcpkg_target_dir/vcpkg/bootstrap-vcpkg.sh
$vcpkg_target_dir/vcpkg/vcpkg integrate install



# Get absolute path to current directory of this script realpath
CURRENT_FILE=$(realpath "${BASH_SOURCE:-$0}")
CURRENT_DIR=$(dirname "${CURRENT_FILE}")

# Install libraries from txt file
while read library; do
  $vcpkg_target_dir/vcpkg/vcpkg install $library
done < $CURRENT_DIR/vcpkg_dependencies.txt
