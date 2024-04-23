# TODO SET PROPER VALUES BEFORE SUBMIT
BUILD_TYPE=Debug
PARALEL=-j 14

.PHONY: build run clean

build:
	if [ ! -f build/Makefile ]; then \
		mkdir build; \
		cd build \
			&& cmake ../src -DCMAKE_BUILD_TYPE=$(BUILD_TYPE); \
	fi
	cd build && $(MAKE) $(PARALEL)

run: build
	build/icp-robots

clean:
	-rm -r build
