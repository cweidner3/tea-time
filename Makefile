
.PHONY: default cmake clean full-clean

default: cmake
	$(MAKE) -C build

cmake: CMakeLists.txt
	@mkdir -p build; \
		cd build; \
		cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=true \
		      -DCMAKE_VERBOSE_MAKEFILE=true \
			  -DCMAKE_INSTALL_PREFIX=/usr ..;

clean:
	$(MAKE) -C build clean

full-clean:
	@rm -rf build

