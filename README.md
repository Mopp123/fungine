

## Dependancies
* GLFW
* GLEW
* stb_image
* Freetype
* HarfBuzz (dependancy when building Freetype on unix)
* Assimp


## Build
To build all the submodules some additional dependancies may need installing.

###Unix
To install all required packages
```
make install-lib-dependancies
```

To build as shared library
`make` or `make all`
This does not install the required packages in case you don't want to do that..

To build the engine only without building submodules.
`make build`
This requires all the submodules to be already built though.


**NOTE: MIGHT BE IMPORTANT ON SOME SETUPS!**<br/>
To build GLEW you may need to alter glew/auto/Makefile line 13
`PYTHON ?= python` to `PYTHON ?= python3`

### Linux
**GLFW**<br/>
https://www.glfw.org/docs/latest/compile.html<br/>
Install X11 and Wayland<br/>
`sudo apt install libwayland-dev libxkbcommon-dev xorg-dev`

**GLEW**<br/>
`sudo apt-get install build-essential libxmu-dev libxi-dev libgl-dev`

**Freetype**<br/>
`sudo apt install automake libtool autoconf`<br/>
Built shared lib can be found from objs/.libs/

**HarfBuzz**<br/>
https://harfbuzz.github.io/building.html<br/>
`sudo apt install libfreetype6-dev libglib2.0-dev libcairo2-dev meson pkg-config gtk-doc-tools`<br/>
Also requires gcc and g++
