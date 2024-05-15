#!/bin/bash

# Copies shared libs and include dirs into "target" dir

# Get script's dir
root_dir=$(dirname "$0")
cd $root_dir
root_dir=$(pwd)
cd $root_dir

#target_dir=$root_dir/TEST
target_dir=$root_dir
echo "___TEST___target = $target_dir"

# GLFW
cp -r glfw/include/* $target_dir/include/
cp glfw/build/src/libglfw.so.3 $target_dir/lib/

# GLEW
cp -r glew/include/* $target_dir/include/
cp glew/lib/libGLEW.so.2.2 $target_dir/lib/

# Freetype
# NOTE: Not sure if all these are needed
cp -r freetype/include/* $target_dir/include/
cp freetype/objs/.libs/libfreetype.so.6.20.1 $target_dir/lib/

# Assimp
cp -r assimp/include/* $target_dir/include/
cp assimp/build/include/assimp/config.h $target_dir/include/assimp/
cp assimp/build/bin/libassimp.so.5.4.1 $target_dir/lib/libassimp.so.5
