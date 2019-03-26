#!/bin/bash

#ToDo replace this script with CMake

g++ -o main.exe Simulator/main.cpp Simulator/Vehicle.cpp Simulator/LiDARSensor.cpp Simulator/Beam.cpp Simulator/Occlusions.cpp Simulator/Geometry.cpp -g -std=c++11
