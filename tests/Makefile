# mrt/tests

TOPDIR   ?= ..
PREFIX   ?= $(TOPDIR)/build

CXX      := g++

CXXFLAGS += -std=c++17 -I$(PREFIX)/include
LDFLAGS  += -L$(PREFIX)/lib -lmrt

SRC      := main.cc
TARGET   := $(PREFIX)/bin/mrt_test

.PHONY: test

test: build
	$(PREFIX)/bin/mrt_test

build:
	$(info Building $(TARGET))
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SRC) -o $(TARGET)

$(V).SILENT:

