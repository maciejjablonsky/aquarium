#!/bin/bash
mkdir -p build
cd build
cmake ..
make
mv aquarium ..
cd ..
./aquarium config.xml
