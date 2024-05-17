#!/bin/bash

# Copies submodule 3rd party libs into our combined lib dir
# NOTE: Requires first building all submodules

# Get script's dir
root_dir=$(dirname "$0")
cd $root_dir
root_dir=$(pwd)
cd $root_dir

#target_dir=$root_dir/TEST
target_dir="$root_dir/lib"
echo "Assigned target: $target_dir"

# GLFW
cp glfw/build/src/libglfw.so.3 $target_dir/

# GLEW
cp glew/lib/libGLEW.so.2.2 $target_dir/

# Freetype
cp freetype/objs/.libs/libfreetype.so.6.20.1 $target_dir/

# Assimp
cp assimp/build/bin/libassimp.so.5.4.1 $target_dir/libassimp.so.5
