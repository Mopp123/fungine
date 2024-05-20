SHELL = /bin/bash
build_dir := ./build
shared_build_name := libfungine-engine.so


all: build-libs build


install-lib-dependancies:
	./install-3rd-party-deps.sh

build-libs:
	./build-3rd-party-libs.sh

build: $(wildcard ./src/*) CMakeLists.txt
	./compile-header-file.sh
	cmake -S . -B $(build_dir)
	cmake --build $(build_dir)

compile-header:
	./compile-header-file.sh

clean:
	rm -rf $(build_dir)
	./clean-3rd-party-dirs.sh
