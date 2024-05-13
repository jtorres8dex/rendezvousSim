cmake_minimum_required(VERSION 3.10) # Specify the minimum version of CMake required

# Set the project name and version
project(Simulation VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

CXXFLAGS += -I/your_project/libs/Eigen

include_directories("libs/")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Add an executable target called "" to be built from the specified source files
add_executable(firstRun ../src/firstRun.cpp ../src/simulation.cpp ../src/Vehicle.cpp)

# If there are any libraries that need to be linked, you might use target_link_libraries. 
# For example, if you had a library named "mathlib", you would uncomment and modify the following line:
# target_link_libraries(simulation PRIVATE mathlib)
