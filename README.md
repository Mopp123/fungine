## Build
To build all the submodules some additional dependancies may need installing.

### Unix
To install all required packages
```
sudo make install-lib-dependancies
```

To build
`make` or `make all`
This does not install the required packages in case you already have those or
don't want to install for some other reason.

To build the engine only without building submodules.
`make build`
This requires all the submodules to be already built though.

These creates `build` directory containing the build as shared library.

To clean up build and submodules' builds run `make clean`
