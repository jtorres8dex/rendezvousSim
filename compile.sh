#!/bin/bash

# for file in *.cpp; do
#     if [ -f "$file" ]; then
#         echo "Compiling $file..."
#         g++ -o "${file%.cpp}" "$file"
#         if [ $? -eq 0 ]; then
#             echo "Compilation successful."
#         else
#             echo "Compilation failed."
#         fi
#     fi
# done
g++ -c vehicle.cpp -o vehicle.o
g++ -c simulation.cpp -o simulation.o
g++ vehicle.o simulation.o -o simulation
