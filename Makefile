BUILD_TYPE=Release
PARALEL=-j $(shell nproc)

ARCHIVE=xsleza26-xstigl00.tar.gz

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

.PHONY: pack
pack:
	tar czf $(ARCHIVE) -- src/* Doxyfile Makefile README.txt class-diagram.pdf

.PHONY: clean
clean:
	-rm -r build doc $(ARCHIVE)
