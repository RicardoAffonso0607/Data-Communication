#!/bin/bash

# Data-Communication Project — Automated Setup Script for Linux
# This script detects your Linux distribution, installs dependencies, compiles, and optionally runs the project.
# Supported: Debian/Ubuntu, Fedora, Arch Linux

set -e  # Exit on first error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Print helper functions
print_header() {
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}========================================${NC}"
}

print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_error() {
    echo -e "${RED}✗ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠ $1${NC}"
}

print_info() {
    echo -e "${BLUE}ℹ $1${NC}"
}

# Detect Linux distribution
detect_distro() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        DISTRO=$ID
    elif [ -f /etc/lsb-release ]; then
        . /etc/lsb-release
        DISTRO=$(echo $DISTRIB_ID | tr '[:upper:]' '[:lower:]')
    else
        DISTRO="unknown"
    fi

    case "$DISTRO" in
        ubuntu|debian)
            echo "debian"
            ;;
        fedora)
            echo "fedora"
            ;;
        arch|manjaro)
            echo "arch"
            ;;
        *)
            echo "unknown"
            ;;
    esac
}

# Install dependencies for Debian/Ubuntu
install_deps_debian() {
    print_header "Installing dependencies (Debian/Ubuntu)"
    
    print_info "Updating package manager..."
    sudo apt update
    sudo apt upgrade -y
    
    print_info "Installing build tools and libraries..."
    sudo apt install -y \
        build-essential \
        git \
        cmake \
        pkg-config \
        libglfw3-dev \
        libgl1-mesa-dev \
        libglu1-mesa-dev \
        libx11-dev \
        libxrandr-dev \
        libxinerama-dev \
        libxcursor-dev \
        libxi-dev
    
    print_success "Dependencies installed"
}

# Install dependencies for Fedora
install_deps_fedora() {
    print_header "Installing dependencies (Fedora)"
    
    print_info "Installing build tools and libraries..."
    sudo dnf install -y \
        @development-tools \
        git \
        cmake \
        pkg-config \
        glfw-devel \
        mesa-libGL-devel \
        libX11-devel \
        libXrandr-devel \
        libXinerama-devel \
        libXcursor-devel \
        libXi-devel
    
    print_success "Dependencies installed"
}

# Install dependencies for Arch Linux
install_deps_arch() {
    print_header "Installing dependencies (Arch Linux)"
    
    print_info "Installing build tools and libraries..."
    sudo pacman -Syu --needed \
        base-devel \
        git \
        cmake \
        pkg-config \
        glfw-x11 \
        mesa \
        libx11 \
        libxrandr \
        libxinerama \
        libxcursor \
        libxi
    
    print_success "Dependencies installed"
}

# Compile the project
compile_project() {
    print_header "Compiling Data-Communication"
    
    if [ ! -f "Makefile" ]; then
        print_error "Makefile not found! Are you in the project root directory?"
        exit 1
    fi
    
    print_info "Running 'make clean'..."
    make clean
    
    print_info "Running 'make all'..."
    make all
    
    if [ -f "./build/meu_programa" ]; then
        print_success "Compilation successful! Executable: ./build/meu_programa"
    else
        print_error "Compilation failed. Check the error messages above."
        exit 1
    fi
}

# Verify config.txt exists
setup_config() {
    print_header "Checking configuration"
    
    if [ ! -f "config.txt" ]; then
        print_warning "config.txt not found, creating default..."
        cat > config.txt << EOF
# Data-Communication Configuration
RECEIVER_IP=127.0.0.1
PORT=4444
EOF
        print_success "config.txt created with defaults (localhost testing)"
    else
        print_success "config.txt exists"
    fi
}

# Ask user if they want to run the app
ask_run() {
    print_header "Setup Complete"
    print_success "All dependencies installed and project compiled!"
    echo ""
    echo "Next steps:"
    echo "1. Edit config.txt if needed (default: localhost testing)"
    echo "2. To run receiver: ./build/meu_programa"
    echo "3. To run sender: ./build/meu_programa (in another terminal)"
    echo ""
    
    read -p "Do you want to run the application now? (y/n): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        print_info "Starting application..."
        ./build/meu_programa
    else
        print_info "Skipped. You can run './build/meu_programa' later."
    fi
}

# Main script
main() {
    print_header "Data-Communication — Linux Setup Script"
    
    # Check if we're in the right directory
    if [ ! -f "Makefile" ] || [ ! -d "src" ]; then
        print_error "This script must be run from the project root directory!"
        echo "Please navigate to the Data-Communication directory and try again."
        exit 1
    fi
    
    DETECTED_DISTRO=$(detect_distro)
    
    if [ "$DETECTED_DISTRO" = "unknown" ]; then
        print_error "Could not detect your Linux distribution."
        echo "Please visit the manual installation guide:"
        echo "  https://github.com/RicardoAffonso0607/Data-Communication"
        exit 1
    fi
    
    print_info "Detected distribution: $DETECTED_DISTRO"
    
    # Install dependencies based on distro
    case "$DETECTED_DISTRO" in
        debian)
            install_deps_debian
            ;;
        fedora)
            install_deps_fedora
            ;;
        arch)
            install_deps_arch
            ;;
    esac
    
    # Compile project
    compile_project
    
    # Setup config
    setup_config
    
    # Ask to run
    ask_run
}

# Run main
main
