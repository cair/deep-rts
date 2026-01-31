#!/bin/bash
#
# DeepRTS Installation Script
# For Ubuntu/Debian/Linux Mint systems
#

set -e  # Exit on error

echo "========================================"
echo "DeepRTS Installation Script"
echo "========================================"
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VENV_DIR="$SCRIPT_DIR/venv"

# Function to print status
status() {
    echo -e "${GREEN}[✓]${NC} $1"
}

warning() {
    echo -e "${YELLOW}[!]${NC} $1"
}

error() {
    echo -e "${RED}[✗]${NC} $1"
}

# Check if running as root for apt commands
check_sudo() {
    if command -v sudo &> /dev/null; then
        SUDO="sudo"
    else
        SUDO=""
    fi
}

# Step 1: Install system dependencies
echo "Step 1: Installing system dependencies..."
echo ""

check_sudo

$SUDO apt-get update -qq

PACKAGES="cmake build-essential ninja-build git curl zip unzip tar pkg-config"
PACKAGES="$PACKAGES libopencv-dev python3-pip python3-dev python3-venv python3-full"
PACKAGES="$PACKAGES libdbus-1-dev libxi-dev libxtst-dev"
# Required by vcpkg for building gperf, gettext, and other dependencies
PACKAGES="$PACKAGES autoconf autoconf-archive automake libtool bison flex gperf"
# X11 development libraries required by cairo
PACKAGES="$PACKAGES libx11-dev libxft-dev libxext-dev libxrender-dev libxrandr-dev"

echo "Installing: $PACKAGES"
$SUDO apt-get install -y $PACKAGES

status "System dependencies installed"
echo ""

# Step 2: Create virtual environment
echo "Step 2: Creating Python virtual environment..."

cd "$SCRIPT_DIR"

if [ -d "$VENV_DIR" ]; then
    warning "Virtual environment already exists at $VENV_DIR"
    read -p "Delete and recreate? (y/N) " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        rm -rf "$VENV_DIR"
        python3 -m venv "$VENV_DIR"
        status "Virtual environment recreated"
    else
        status "Using existing virtual environment"
    fi
else
    python3 -m venv "$VENV_DIR"
    status "Virtual environment created at $VENV_DIR"
fi

# Activate virtual environment
source "$VENV_DIR/bin/activate"

echo "Python: $(python --version)"
echo "Pip: $(pip --version)"
status "Virtual environment activated"
echo ""

# Step 3: Install Python dependencies
echo "Step 3: Installing Python dependencies..."

pip install --upgrade pip setuptools wheel
pip install numpy opencv-python

status "Python dependencies installed"
echo ""

# Step 4: Build and install DeepRTS
echo "Step 4: Building DeepRTS (this may take 5-15 minutes)..."
echo ""

echo "Working directory: $SCRIPT_DIR"
echo ""

# Clean any previous build artifacts
if [ -d "build" ]; then
    warning "Removing old build directory..."
    rm -rf build
fi

if [ -d "_skbuild" ]; then
    warning "Removing old _skbuild directory..."
    rm -rf _skbuild
fi

# Install in development mode
echo "Running: pip install -e ."
echo ""
pip install -e . 2>&1 | tee build.log

status "DeepRTS built and installed!"
echo ""

# Step 5: Verify installation
echo "Step 5: Verifying installation..."

if python -c "from DeepRTS import Engine; print('DeepRTS Engine loaded successfully!')" 2>/dev/null; then
    status "Installation verified!"
    echo ""
    echo "========================================"
    echo -e "${GREEN}Installation complete!${NC}"
    echo "========================================"
    echo ""
    echo "To use DeepRTS, first activate the virtual environment:"
    echo ""
    echo "  source venv/bin/activate"
    echo ""
    echo "Then run:"
    echo ""
    echo "  python examples/demo_1v1.py"
    echo ""
    echo "To deactivate the virtual environment when done:"
    echo ""
    echo "  deactivate"
    echo ""
else
    error "Verification failed!"
    echo ""
    echo "Check build.log for errors."
    echo "Common issues:"
    echo "  - Missing OpenCV: sudo apt install libopencv-dev"
    echo "  - CMake too old: sudo apt install cmake"
    echo ""
    exit 1
fi
