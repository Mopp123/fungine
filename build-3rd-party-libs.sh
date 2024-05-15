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

# build GLEW
cd glew/auto
echo "Making glew(auto)..."
make
cd ..
echo "Making glew..."
make
#make install
#make clean


# NOTE: BELOW NOT TESTED YET!

# build Freetype and HarfBuzz
# initial Freetype..
echo "Building freetype(without harfbuzz)"
cd ../freetype
./autogen.sh
./configure --without-harfbuzz
make
# build HarfBuzz
echo "Building harfbuzz"
cd ../harfbuzz
meson build
meson compile -C build
# finalize building Freetype
cd ../freetype
make distclean
./configure
make

# Assimp
echo "Building assimp"
cd ../assimp
cmake -S . -B ./build -D ASSIMP_INSTALL=OFF
cmake --build ./build
