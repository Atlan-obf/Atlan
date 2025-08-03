#!/bin/bash

# AI Assistant Build Script (GTK4 Version)
# Automaticky kompiluje AI Assistant aplikáciu s GTK4

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
    
    # Check GTK4 and gtkmm4
    if command -v pkg-config &> /dev/null; then
        if pkg-config --exists gtk4; then
            GTK_VERSION=$(pkg-config --modversion gtk4)
            print_status "GTK4 version: $GTK_VERSION"
        else
            print_warning "GTK4 not found via pkg-config"
        fi
        
        if pkg-config --exists gtkmm-4.0; then
            GTKMM_VERSION=$(pkg-config --modversion gtkmm-4.0)
            print_status "gtkmm4 version: $GTKMM_VERSION"
        else
            print_warning "gtkmm-4.0 not found via pkg-config"
        fi
    else
        print_warning "pkg-config not found. Cannot check GTK4/gtkmm4 versions."
    fi
    
    # Check libcurl
    if pkg-config --exists libcurl; then
        CURL_VERSION=$(pkg-config --modversion libcurl)
        print_status "libcurl version: $CURL_VERSION"
    else
        print_warning "libcurl not found via pkg-config"
    fi
    
    # Check jsoncpp
    if pkg-config --exists jsoncpp; then
        JSON_VERSION=$(pkg-config --modversion jsoncpp)
        print_status "jsoncpp version: $JSON_VERSION"
    else
        print_warning "jsoncpp not found via pkg-config"
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
                sudo apt install -y build-essential cmake git pkg-config
                sudo apt install -y libgtk-4-dev libgtkmm-4.0-dev
                sudo apt install -y libcurl4-openssl-dev libjsoncpp-dev
            elif command -v yum &> /dev/null; then
                print_status "Using yum package manager..."
                sudo yum install -y gcc-c++ cmake git pkgconfig
                sudo yum install -y gtk4-devel gtkmm4-devel
                sudo yum install -y libcurl-devel jsoncpp-devel
            elif command -v pacman &> /dev/null; then
                print_status "Using pacman package manager..."
                sudo pacman -S --noconfirm gcc cmake git pkgconf
                sudo pacman -S --noconfirm gtk4 gtkmm-4.0
                sudo pacman -S --noconfirm curl jsoncpp
            elif command -v dnf &> /dev/null; then
                print_status "Using dnf package manager..."
                sudo dnf install -y gcc-c++ cmake git pkgconfig
                sudo dnf install -y gtk4-devel gtkmm40-devel
                sudo dnf install -y libcurl-devel jsoncpp-devel
            else
                print_warning "Unknown package manager. Please install dependencies manually:"
                print_status "Required packages: build-essential cmake git pkg-config"
                print_status "GTK4: libgtk-4-dev libgtkmm-4.0-dev"
                print_status "Other: libcurl4-openssl-dev libjsoncpp-dev"
            fi
            ;;
        "macos")
            if command -v brew &> /dev/null; then
                print_status "Using Homebrew..."
                brew install cmake pkg-config
                brew install gtk4 gtkmm4
                brew install curl jsoncpp
            else
                print_error "Homebrew not found. Please install it first:"
                print_status "/bin/bash -c \"\$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\""
                exit 1
            fi
            ;;
        "windows")
            print_warning "On Windows, please ensure you have MSYS2 with required packages installed."
            print_status "Run these commands in MSYS2:"
            print_status "pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake"
            print_status "pacman -S mingw-w64-x86_64-gtk4 mingw-w64-x86_64-gtkmm4"
            print_status "pacman -S mingw-w64-x86_64-curl mingw-w64-x86_64-jsoncpp"
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
        
        # Check dependencies
        if [[ "$OS" == "linux" ]] && command -v ldd &> /dev/null; then
            print_status "Checking dynamic dependencies..."
            ldd "$EXECUTABLE" | grep -E "(gtk|gtkmm|curl|json)" | head -5
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
    echo "AI Assistant Build Script (GTK4 Version)"
    echo ""
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  --help          Show this help message"
    echo "  --install-deps  Install system dependencies (GTK4, gtkmm4, etc.)"
    echo "  --clean         Clean build directory before building"
    echo "  --run           Run the application after building"
    echo "  --debug         Build in debug mode"
    echo ""
    echo "Dependencies:"
    echo "  - GTK4 and gtkmm4 (GUI framework)"
    echo "  - libcurl (HTTP requests)"
    echo "  - jsoncpp (JSON parsing)"
    echo "  - CMake 3.16+ and C++17 compiler"
    echo ""
    echo "Examples:"
    echo "  $0                    # Build the project"
    echo "  $0 --clean --run     # Clean, build and run"
    echo "  $0 --install-deps    # Install dependencies and build"
}

# Main function
main() {
    print_status "AI Assistant Build Script (GTK4 Version) Starting..."
    
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
    print_status "This version uses GTK4 instead of Qt6 - completely free and open source!"
    
    # Run if requested
    if [ "$RUN" = true ]; then
        cd build
        run_application --run
    fi
}

# Run main function with all arguments
main "$@"