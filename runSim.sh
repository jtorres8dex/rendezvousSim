#!/bin/bash

./compile.sh

./executables/TEST_EXECUTABLE 

python3 visualize/plotter.py logs/SIMULATION_TEST.csv