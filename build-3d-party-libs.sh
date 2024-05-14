#!/bin/bash

# Get script's dir
root_dir=$( dirname "$0" )
cd $root_dir
root_dir=$( pwd )

# build glfw
echo "Building glfw..."
cd glfw/
cmake -S . -B build -D BUILD_SHARED_LIBS=ON
cmake --build ./build/

cd $root_dir

# build glew
cd glew/auto
echo "Making glew(auto)..."
make

cd ..
echo "Making glew..."
make
#make install
#make clean
