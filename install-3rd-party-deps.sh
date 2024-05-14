#!/bin/bash

# NOTE: This should be run with sudo
apt-get update

apt-get install build-essential libxmu-dev libxi-dev libgl-dev

# For windowing on linux
apt-get install libwayland-dev libxkbcommon-dev xorg-dev

# For building freetype and harfbuzz
apt-get install automake libtool autoconf libfreetype6-dev libglib2.0-dev libcairo2-dev meson pkg-config gtk-doc-tools
