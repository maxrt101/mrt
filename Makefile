# mrt c++ lib

export TOPDIR := $(shell pwd)
export PREFIX ?= $(TOPDIR)/build

.PHONY: build

build:
	$(info Building mrt)
	make -C src

test:
	make -C tests

clean:
	make -C src clean

$(V).SILENT:

