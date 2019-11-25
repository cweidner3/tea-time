
.PHONY: default cmake clean full-clean install

default: cmake
	$(MAKE) -C build

cmake: CMakeLists.txt
	@mkdir -p build; \
		cd build; \
		cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=true -DCMAKE_VERBOSE_MAKEFILE=true ..;

clean:
	$(MAKE) -C build clean

full-clean:
	@rm -rf build

install:
	install -d $(DESTDIR)$(PREFIX)/usr/bin
	install -d $(DESTDIR)$(PREFIX)/usr/share/applications
	install -d $(DESTDIR)$(PREFIX)/usr/share/icons/hicolor/96x96
	install -d $(DESTDIR)$(PREFIX)/usr/share/icons/hicolor/48x48
	install -d $(DESTDIR)$(PREFIX)/usr/share/icons/hicolor/24x24
	install -d $(DESTDIR)$(PREFIX)/usr/share/icons/hicolor/16x16
	install -m 0755 -t $(DESTDIR)$(PREFIX)/usr/bin build/tea-time
	install -m 0644 -t $(DESTDIR)$(PREFIX)/usr/share/applications save/tea-time.desktop
	install -m 0644 -t $(DESTDIR)$(PREFIX)/usr/share/icons/hicolor/96x96 icon-dev/96x96/tea-time.png
	install -m 0644 -t $(DESTDIR)$(PREFIX)/usr/share/icons/hicolor/48x48 icon-dev/48x48/tea-time.png
	install -m 0644 -t $(DESTDIR)$(PREFIX)/usr/share/icons/hicolor/24x24 icon-dev/24x24/tea-time.png
	install -m 0644 -t $(DESTDIR)$(PREFIX)/usr/share/icons/hicolor/16x16 icon-dev/16x16/tea-time.png

