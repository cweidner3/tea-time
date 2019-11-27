
.PHONY: default all cmake clean full-clean test

default: cmake
	$(MAKE) -C build

all: cmake
	$(MAKE) -C build all

cmake: CMakeLists.txt
	@mkdir -p build; \
		cd build; \
		cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=true \
		      -DCMAKE_VERBOSE_MAKEFILE=true \
		      -DCMAKE_C_COMPILER=$(shell which gcc) \
		      -DCMAKE_CXX_COMPILER=$(shell which g++) \
			  -DCMAKE_INSTALL_PREFIX=/usr ..;

clean:
	$(MAKE) -C build clean

full-clean:
	@rm -rf build

test:
	$(MAKE) -C build test

