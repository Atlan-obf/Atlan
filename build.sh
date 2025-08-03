#!/bin/bash

# AI Assistant Build Script
# Automaticky kompiluje AI Assistant aplikáciu

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Detect OS
detect_os() {
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        OS="linux"
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        OS="macos"
    elif [[ "$OSTYPE" == "msys" ]]; then
        OS="windows"
    else
        OS="unknown"
    fi
    print_status "Detected OS: $OS"
}

# Check dependencies
check_dependencies() {
    print_status "Checking dependencies..."
    
    # Check CMake
    if ! command -v cmake &> /dev/null; then
        print_error "CMake not found. Please install CMake 3.16 or newer."
        exit 1
    fi
    
    CMAKE_VERSION=$(cmake --version | head -n1 | cut -d" " -f3)
    print_status "CMake version: $CMAKE_VERSION"
    
    # Check compiler
    if command -v g++ &> /dev/null; then
        COMPILER="g++"
        COMPILER_VERSION=$(g++ --version | head -n1)
    elif command -v clang++ &> /dev/null; then
        COMPILER="clang++"
        COMPILER_VERSION=$(clang++ --version | head -n1)
    else
        print_error "No suitable C++ compiler found (g++ or clang++)"
        exit 1
    fi
    
    print_status "Compiler: $COMPILER_VERSION"
    
    # Check Qt6
    if command -v qmake6 &> /dev/null; then
        QT_VERSION=$(qmake6 --version | grep "Using Qt version" | cut -d" " -f4)
        print_status "Qt version: $QT_VERSION"
    elif command -v qmake &> /dev/null; then
        QT_VERSION=$(qmake --version | grep "Using Qt version" | cut -d" " -f4)
        print_status "Qt version: $QT_VERSION"
    else
        print_warning "qmake not found in PATH. Qt6 may not be properly installed."
    fi
}

# Install dependencies based on OS
install_dependencies() {
    print_status "Installing dependencies for $OS..."
    
    case $OS in
        "linux")
            if command -v apt &> /dev/null; then
                print_status "Using apt package manager..."
                sudo apt update
                sudo apt install -y build-essential cmake git
                sudo apt install -y qt6-base-dev qt6-tools-dev libcurl4-openssl-dev
            elif command -v yum &> /dev/null; then
                print_status "Using yum package manager..."
                sudo yum install -y gcc-c++ cmake git
                sudo yum install -y qt6-qtbase-devel libcurl-devel
            elif command -v pacman &> /dev/null; then
                print_status "Using pacman package manager..."
                sudo pacman -S --noconfirm gcc cmake git
                sudo pacman -S --noconfirm qt6-base curl
            else
                print_warning "Unknown package manager. Please install dependencies manually."
            fi
            ;;
        "macos")
            if command -v brew &> /dev/null; then
                print_status "Using Homebrew..."
                brew install cmake qt6 curl
            else
                print_error "Homebrew not found. Please install it first or install dependencies manually."
                exit 1
            fi
            ;;
        "windows")
            print_warning "On Windows, please ensure you have MSYS2 with required packages installed."
            print_status "Run: pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-qt6 mingw-w64-x86_64-curl"
            ;;
        *)
            print_error "Unsupported OS: $OS"
            exit 1
            ;;
    esac
}

# Clean build directory
clean_build() {
    if [ -d "build" ]; then
        print_status "Cleaning existing build directory..."
        rm -rf build
    fi
}

# Configure project
configure_project() {
    print_status "Configuring project..."
    mkdir -p build
    cd build
    
    # Configure with CMake
    if [[ "$OS" == "windows" ]]; then
        cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
    else
        cmake .. -DCMAKE_BUILD_TYPE=Release
    fi
    
    if [ $? -eq 0 ]; then
        print_success "Project configured successfully"
    else
        print_error "Configuration failed"
        exit 1
    fi
}

# Build project
build_project() {
    print_status "Building project..."
    
    # Detect number of CPU cores
    if [[ "$OS" == "linux" ]]; then
        CORES=$(nproc)
    elif [[ "$OS" == "macos" ]]; then
        CORES=$(sysctl -n hw.ncpu)
    else
        CORES=4  # Default for Windows
    fi
    
    print_status "Using $CORES parallel jobs"
    
    if [[ "$OS" == "windows" ]]; then
        mingw32-make -j$CORES
    else
        make -j$CORES
    fi
    
    if [ $? -eq 0 ]; then
        print_success "Build completed successfully"
    else
        print_error "Build failed"
        exit 1
    fi
}

# Post-build actions
post_build() {
    print_status "Performing post-build actions..."
    
    # Check if executable was created
    if [[ "$OS" == "windows" ]]; then
        EXECUTABLE="AIAssistant.exe"
    else
        EXECUTABLE="AIAssistant"
    fi
    
    if [ -f "$EXECUTABLE" ]; then
        print_success "Executable created: $EXECUTABLE"
        
        # Make executable on Unix systems
        if [[ "$OS" != "windows" ]]; then
            chmod +x "$EXECUTABLE"
        fi
        
        # Get file size
        if command -v du &> /dev/null; then
            SIZE=$(du -h "$EXECUTABLE" | cut -f1)
            print_status "Executable size: $SIZE"
        fi
    else
        print_error "Executable not found after build"
        exit 1
    fi
}

# Run application
run_application() {
    if [ "$1" == "--run" ]; then
        print_status "Starting AI Assistant..."
        if [[ "$OS" == "windows" ]]; then
            ./AIAssistant.exe
        else
            ./AIAssistant
        fi
    fi
}

# Show usage
show_usage() {
    echo "AI Assistant Build Script"
    echo ""
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  --help          Show this help message"
    echo "  --install-deps  Install system dependencies"
    echo "  --clean         Clean build directory before building"
    echo "  --run           Run the application after building"
    echo "  --debug         Build in debug mode"
    echo ""
    echo "Examples:"
    echo "  $0                    # Build the project"
    echo "  $0 --clean --run     # Clean, build and run"
    echo "  $0 --install-deps    # Install dependencies and build"
}

# Main function
main() {
    print_status "AI Assistant Build Script Starting..."
    
    # Parse command line arguments
    INSTALL_DEPS=false
    CLEAN=false
    RUN=false
    DEBUG=false
    
    while [[ $# -gt 0 ]]; do
        case $1 in
            --help)
                show_usage
                exit 0
                ;;
            --install-deps)
                INSTALL_DEPS=true
                shift
                ;;
            --clean)
                CLEAN=true
                shift
                ;;
            --run)
                RUN=true
                shift
                ;;
            --debug)
                DEBUG=true
                shift
                ;;
            *)
                print_error "Unknown option: $1"
                show_usage
                exit 1
                ;;
        esac
    done
    
    # Detect OS
    detect_os
    
    # Install dependencies if requested
    if [ "$INSTALL_DEPS" = true ]; then
        install_dependencies
    fi
    
    # Check dependencies
    check_dependencies
    
    # Clean if requested
    if [ "$CLEAN" = true ]; then
        clean_build
    fi
    
    # Configure project
    configure_project
    
    # Build project
    build_project
    
    # Post-build actions
    post_build
    
    # Go back to project root
    cd ..
    
    print_success "Build completed successfully!"
    print_status "Executable location: build/AIAssistant"
    
    # Run if requested
    if [ "$RUN" = true ]; then
        cd build
        run_application --run
    fi
}

# Run main function with all arguments
main "$@"