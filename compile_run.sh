#!/usr/bin/env bash
set -e

# Default build type
BUILD_TYPE="Debug"
BUILD_DIR="build"

# Function to show usage
show_usage() {
    echo "Usage: $0 [OPTIONS]"
    echo "Options:"
    echo "  -d, --debug           Build in Debug mode (default)"
    echo "  -r, --release         Build in Release mode"
    echo "  -rd, --release-debug  Build in RelWithDebInfo mode (Release with debug info)"
    echo "  -h, --help            Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0                    # Build in Debug mode"
    echo "  $0 -r                 # Build in Release mode"
    echo "  $0 --release-debug    # Build in Release with debug info"
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -d|--debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        -r|--release)
            BUILD_TYPE="Release"
            shift
            ;;
        -rd|--release-debug)
            BUILD_TYPE="RelWithDebInfo"
            shift
            ;;
        -h|--help)
            show_usage
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            show_usage
            exit 1
            ;;
    esac
done

# Update build directory name to include build type
BUILD_DIR="build_${BUILD_TYPE,,}"  # Convert to lowercase

echo "Building in $BUILD_TYPE mode..."
echo "Build directory: $BUILD_DIR/"

# Create build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory: $BUILD_DIR/"
    mkdir "$BUILD_DIR"
fi

echo "Cleaning $BUILD_DIR"
rm -rf "$BUILD_DIR"/*

echo "Running CMake with build type: $BUILD_TYPE"
cd "$BUILD_DIR"
cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" ..

echo "Running make..."
make

echo "Build completed successfully!"
echo "Launching Game..."
./MyGame
