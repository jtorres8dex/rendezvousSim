#!/bin/bash

# Define your Docker image name
IMAGE_NAME="your_image_name"

# Build the Docker image
echo "Building Docker image: $IMAGE_NAME"
docker build -t $IMAGE_NAME .

# Check if the build was successful
if [ $? -eq 0 ]; then
    echo "Build succeeded. Running Docker container from image: $IMAGE_NAME"

    # Run the Docker container
    # Here you can add any `docker run` options you need.
    # For example, to remove the container after it exits, use the --rm flag.
    # If you need to specify ports, use the -p host_port:container_port
    # If your application requires more memory than the default, consider using the -m option.
    docker run --rm $IMAGE_NAME
else
    echo "Build failed. Exiting."
    exit 1
fi
