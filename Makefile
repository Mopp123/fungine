SHELL = /bin/bash
build_dir := ./build
shared_build_name := libfungine-engine.so


all: build-libs create-include-dir build


install-lib-dependancies:
	./install-3rd-party-deps.sh

build-libs:
	./build-3rd-party-libs.sh

build: $(wildcard ./src/*) CMakeLists.txt
	cmake -S . -B $(build_dir)
	cmake --build $(build_dir)

create-include-dir:
	./create-include-dir.sh

#copy-lib-includes:
#	./copy-3rd-party-includes.sh

create-lib-dir:
	# if already exists -> delete it
	if [ -d "./lib" ]; then rm -rf lib; fi
	mkdir lib
	cp $(build_dir)/$(shared_build_name) ./lib/
	./copy-3rd-party-libs.sh

clean:
	rm -rf $(build_dir)
	./clean-3rd-party-dirs.sh
