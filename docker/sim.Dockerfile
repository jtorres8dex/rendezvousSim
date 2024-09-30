# Use the official Ubuntu base image
FROM ubuntu:latest

# Set the working directory in the container
WORKDIR /usr/src/app

# Install build essentials and other dependencies
# For a C++ project, you might need build-essential, cmake.
# Add other dependencies specific to your project
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

# Copy your project files into the container
COPY . .

# Your build commands here
# For example, if you use CMake:
RUN mkdir -p build && \
    cd build && \
    cmake .. && \
    make

# Specify the command to run your application
# Replace `./app` with the path to the executable produced by your build
CMD ["./build/firstRun"]
