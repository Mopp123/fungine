#!/bin/bash

# Copies submodule 3rd party include dirs into our combined include dir
# NOTE: Requires first building all submodules
# NOTE: Requires first running create-include-dir.sh

# Get script's dir
root_dir=$(dirname "$0")
cd $root_dir
root_dir=$(pwd)
cd $root_dir

#target_dir=$root_dir/TEST
target_dir=$root_dir/include

# GLFW
cp -r glfw/include/* $target_dir/

# GLEW
cp -r glew/include/* $target_dir/

# Freetype
# NOTE: Not sure if all these are needed
cp -r freetype/include/* $target_dir/

# Assimp
cp -r assimp/include/* $target_dir/
cp assimp/build/include/assimp/config.h $target_dir/assimp/
