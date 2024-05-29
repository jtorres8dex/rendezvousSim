#!/bin/bash

# Define the output executable name
EXECUTABLE="simulation"

# Clean up previous compilation artifacts
echo "Cleaning up old compilation artifacts..."
rm -f vehicle.o $EXECUTABLE
echo "Cleanup done."

echo "Compiling component files..."
g++ -c src/Vehicle.cpp -o build/vehicle.o
g++ -c src/kinematics.cpp -o build/kinematics.o

# Check if the compilation was successful
if [ $? -ne 0 ]; then
    echo 
    echo "Compilation of componets failed. Exiting."
    exit 1
fi

# Compile simulation.cpp along with the vehicle.o object file,
# and any other .cpp files you need to compile, generating the 'simulation' executable.
# Note: Add other .cpp files as needed
echo "Compiling simulation.cpp and linking..."
g++ src/simulation.cpp build/vehicle.o -o $EXECUTABLE

# If compilation succeeds, print a success message
if [ $? -eq 0 ]; then
    echo "Compilation successful. Executable created: $EXECUTABLE"
else
    echo "Compilation failed."
fi
