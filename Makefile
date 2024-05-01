# TODO SET PROPER VALUES BEFORE SUBMIT
BUILD_TYPE=Debug
PARALEL=-j $(shell nproc)

.PHONY: build
build:
	if [ ! -f build/Makefile ]; then \
		mkdir build; \
		cd build \
			&& cmake ../src -DCMAKE_BUILD_TYPE=$(BUILD_TYPE); \
	fi
	cd build && $(MAKE) $(PARALEL)

.PHONY: run
run: build
	build/icp-robots

.PHONY: doxygen
doxygen:
	doxygen

.PHONY: clean
clean:
	-rm -r build doc
