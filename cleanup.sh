#!/bin/bash

# This cleans up all built libs

# Get script's dir
root_dir=$(dirname "$0")
cd $root_dir
root_dir=$(pwd)

# GLFW
# *NOTE: This assumes glfw was built in glfw/build - dir
echo "Cleaning glfw dir: $(pwd)"
cd glfw
rm -rf build

# GLEW
cd ../glew/auto
make clean && make destroy
cd ..
# NOTE: This is not required if using the "build script provided"
# since it's not calling: make install for glew
make uninstall && make clean && make distclean

# Freetype
cd ../freetype
make distclean

# HarfBuzz
cd ../harfbuzz
rm -rf build

