.PHONY: build run clean

build:
	if [ ! -f build/Makefile ]; then \
		mkdir build; \
		cd build && cmake ../src; \
	fi
	cd build && $(MAKE)

run: build
	build/icp-robots

clean:
	rm -r build
