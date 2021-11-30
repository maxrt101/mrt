# mrt/tests

TOPDIR   ?= ..
PREFIX   ?= $(TOPDIR)/build

CXX      := g++

CXXFLAGS += -std=c++17 -I$(PREFIX)/include

SRC      := main.cc
TARGET   := $(PREFIX)/bin/mrt_test

.PHONY: test

test: build
	$(PREFIX)/bin/mrt_test

build: prepare
	$(info Building $(TARGET))
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

prepare:
	mkdir -p $(PREFIX)/bin

$(V).SILENT:
