# Fungine
Old game engine project I used for a graphics programming course.
A lot learnt after working on this project so haven't been continuing
this one for many years.

Originally written using Visual Studio using tabs(\t) as tabs and later
when automating building on linux (tested only on Ubuntu 22.04) and doing
some fixes some places have been changed to use spaces(4) as tabs instead.
..So the code formatting is pretty fucked up.

## Build
To build all the submodules some additional dependancies may need installing.

### Unix
To install all required packages
`sudo make install-lib-dependancies`

To build:
`make` or `make all`
This does not install the required packages in case you already have those or
don't want to install for some other reason.

To build the engine only without building submodules.
`make build`
This requires all the submodules to be already built though.

These creates `build` directory containing the build as shared library.

To clean up build and submodules' builds run `make clean`
