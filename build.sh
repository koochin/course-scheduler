#!/bin/bash

mkdir build && cd build
cmake -B build/ -S .. -DCMAKE_TOOLCHAIN_FILE=/home/liam/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build .
./course-scheduler ../test.txt

# mkdir build
# cmake -B build/ -S . -DCMAKE_TOOLCHAIN_FILE=/home/liam/vcpkg/scripts/buildsystems/vcpkg.cmake
# cd build
# cmake --build .
# ./course-scheduler ../test.txt
