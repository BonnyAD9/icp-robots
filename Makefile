# TODO SET PROPER VALUES BEFORE SUBMIT
BUILD_TYPE=Debug

.PHONY: build run clean

build:
	if [ ! -f build/Makefile ]; then \
		mkdir build; \
		cd build \
			&& cmake ../src -DCMAKE_BUILD_TYPE=$(BUILD_TYPE); \
	fi
	cd build && $(MAKE)

run: build
	build/icp-robots

clean:
	rm -r build
