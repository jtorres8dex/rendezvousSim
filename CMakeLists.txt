cmake_minimum_required(VERSION 3.10) # Specify the minimum version of CMake required

# Set the project name and version
project(Simulation VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add an executable target called "simulation" to be built from the specified source files
add_executable(simulation simulation.cpp Vehicle.cpp)

# If there are any libraries that need to be linked, you might use target_link_libraries. 
# For example, if you had a library named "mathlib", you would uncomment and modify the following line:
# target_link_libraries(simulation PRIVATE mathlib)
