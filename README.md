
## Dependancies
* GLFW
* GLEW
* stb_image
* Freetype
* Assimp

## Build
To build all the used libraries some additional dependancies may need installing.

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
