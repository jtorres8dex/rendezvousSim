cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(Simulation VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find yaml-cpp package
find_package(yaml-cpp REQUIRED)

# Include directories
include_directories(${YAML_CPP_INCLUDE_DIRS})
include_directories("libs/")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Add an executable target called "firstRun" to be built from the specified source files
add_executable(firstRun ../src/firstRun.cpp ../src/simulation.cpp ../src/Vehicle.cpp)


# Suppress warnings
target_compile_options(firstRun PRIVATE -w)

# Link yaml-cpp library
target_link_libraries(firstRun PRIVATE yaml-cpp)
