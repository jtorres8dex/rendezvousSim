#!/bin/bash

# Set some colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
WHITE='\033[0;37m'
NC='\033[0m' # No Color

clear

# Define the build type (Debug/Release)
BUILD_TYPE=$1
if [ -z "$BUILD_TYPE" ]; then
  BUILD_TYPE="Debug"
fi

# Define the output executable name
EXECUTABLE="TEST_EXECUTABLE"

# Clean up previous compilation artifacts
echo -e "${BLUE}Cleaning up old compilation artifacts...${NC}"
rm -f build/$BUILD_TYPE/*.o build/$BUILD_TYPE/$EXECUTABLE
echo -e "${BLUE}Cleanup done.${NC}"
echo

# Create build directory if it doesn't exist
mkdir -p build/$BUILD_TYPE

# Navigate to the build directory
cd build/$BUILD_TYPE

# Run CMake and make
echo -e "${BLUE}Running CMake...${NC}"
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ../..
echo -e "${BLUE}Building the project...${NC}"
make # VERBOSE=1


# Check if the compilation was successful
if [ $? -ne 0 ]; then
    echo -e "${RED}Compilation failed. Exiting.${NC}"
    exit 1
fi
# cd ../..
# Move the executable to the outputs directory
mkdir -p executables
mv Debug/runMe ../../executables/$EXECUTABLE

# If compilation succeeds, print a success message
if [ $? -eq 0 ]; then
    echo -e "${GREEN}Compilation successful. Executable created: outputs/$EXECUTABLE${NC}"
else
    echo -e "${RED}Compilation failed.${NC}"
fi

# Navigate back to the root directory
cd ../..
