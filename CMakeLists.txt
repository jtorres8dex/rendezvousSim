cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(Simulation VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find yaml-cpp package
find_package(yaml-cpp REQUIRED)

# Include directories
# include_directories(${YAML_CPP_INCLUDE_DIRS})s
include_directories("/opt/homebrew/Cellar/yaml-cpp/0.8.0/include")
include_directories("libs/")

link_directories("/opt/homebrew/Cellar/yaml-cpp/0.8.0/lib")

# Set the build type if not specified
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# Define the output directories for different build types
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)

# Add an executable target called "firstRun" to be built from the specified source files
add_executable(runMe src/simulation.cpp src/Vehicle.cpp src/agent.cpp src/loggingTools.cpp)

# Suppress warnings
target_compile_options(runMe PRIVATE -w)

# Link yaml-cpp library
target_link_libraries(runMe PRIVATE yaml-cpp)
