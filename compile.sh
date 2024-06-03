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

# Define the output executable name
EXECUTABLE="TEST_EXECUTABLE"

# Clean up previous compilation artifacts
echo "${BLUE}Cleaning up old compilation artifacts...${NC}"
rm -f build/*.o $EXECUTABLE
echo "Cleanup done."

echo "${BLUE}Compiling component files...${NC}"
g++ -std=c++14 -c src/Vehicle.cpp -o build/vehicle.o
g++ -std=c++14 -c src/kinematics.cpp -o build/kinematics.o
g++ -std=c++14 -c src/simulation.cpp -o build/simulation.o
g++ -std=c++14 -c src/firstRun.cpp -o build/firstRun.o
# Check if the compilation was successful
if [ $? -ne 0 ]; then
    echo -e "${RED}Compilation of components failed. Exiting.${NC}"
    exit 1
fi

# Compile simulation.cpp along with all object files
echo "Compiling simulation.cpp and linking..."
g++ src/firstRun.cpp build/vehicle.o build/kinematics.o build/simulation.o -o outputs/$EXECUTABLE

# If compilation succeeds, print a success message
if [ $? -eq 0 ]; then
    echo -e "${GREEN}Compilation successful. Executable created: $EXECUTABLE${NC}"
else
    echo -e "${RED}Compilation failed.${NC}"
fi
