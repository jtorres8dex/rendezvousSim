# Use Ubuntu as the base image
FROM ubuntu:latest

# Install build tools and dependencies
# g++ for compiling, cmake for building
RUN apt-get update && \
    apt-get install -y \
    g++ \
    cmake \
    make \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory inside the container
WORKDIR /app

# Copy your project files into the Docker image
COPY . /app

# Build your project
# This assumes your CMakeLists.txt is in the project root and the build directory is /app/build
RUN mkdir -p build && \
    cd build && \
    cmake .. && \
    make \
    gdb \
    && rm -rf /var/lib/apt/lists/*

# Specify the command to run your application
# Replace `./your_application` with the path to your compiled binary
# CMD ["./build/your_application"]
