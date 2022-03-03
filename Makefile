# mrt c++ lib

export TOPDIR     := $(shell pwd)
export PREFIX     ?= $(TOPDIR)/build
export INCLUDEDIR := $(PREFIX)/include/mrt

.PHONY: build

$(info Building mrt)

build: install-headers

install-headers: prepare
	cp src/*.h         	$(INCLUDEDIR)
	cp src/net/*.h    	$(INCLUDEDIR)/net
	cp src/net/http/*.h $(INCLUDEDIR)/net/http
	cp src/test/*.h    	$(INCLUDEDIR)/test
	cp src/console/*.h 	$(INCLUDEDIR)/console
	cp src/threads/*.h 	$(INCLUDEDIR)/threads

prepare:
	mkdir -p $(PREFIX)
	mkdir -p $(INCLUDEDIR)
	mkdir -p $(INCLUDEDIR)/net
	mkdir -p $(INCLUDEDIR)/net/http
	mkdir -p $(INCLUDEDIR)/test
	mkdir -p $(INCLUDEDIR)/console
	mkdir -p $(INCLUDEDIR)/threads

test:
	make -C tests

$(V).SILENT:
