cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(Simulation VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find yaml-cpp package
find_package(yaml-cpp REQUIRED)
# Find the Eigen3 package
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# Set the build type if not specified
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# Define the output directories for different build types
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)

# Add an executable target called "runMe" and specify the source files
add_executable(runMe src/sim/simulation.cpp src/physics/Vehicle.cpp
src/agents/followerAgent.cpp libs/loggingTools.cpp libs/graphTheoryTools.cpp libs/global.cpp
src/agents/agentBase.cpp src/agents/leaderAgent.cpp src/agents/agentManager.cpp libs/state.cpp)

target_include_directories(runMe PRIVATE ${CMAKE_SOURCE_DIR}/libs ${CMAKE_SOURCE_DIR}/src)
# Now specify the include directories after defining the target

# Link directories for libraries
include_directories("/opt/homebrew/Cellar/yaml-cpp/0.8.0/include")
# link_directories("/opt/homebrew/Cellar/yaml-cpp/0.8.0/lib")
link_directories(/opt/homebrew/opt/yaml-cpp/include)

# Suppress warnings
target_compile_options(runMe PRIVATE -w)

# Link yaml-cpp and Eigen libraries
target_link_libraries(runMe PRIVATE /opt/homebrew/opt/yaml-cpp/lib/libyaml-cpp.dylib Eigen3::Eigen)
# target_link_libraries(runMe PRIVATE yaml-cpp Eigen3::Eigen)
