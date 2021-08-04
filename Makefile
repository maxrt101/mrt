# mrt c++ lib

export CXX      := g++
export AR       := ar
export CFLAGS   := -c -I./
export CXXFLAGS :=  $(CFLAGS) -std=c++17

BUILDDIR        ?= build
LIBDIR          := $(BUILDDIR)/lib
INCLUDEDIR      := $(BUILDDIR)/include/mrt

TARGET          := $(LIBDIR)/libmrt.a
OBJS            := args/args.o threads/job.o threads/pool.o strutils.o

.PHONY: install

install: compile
	cp optional.h     $(INCLUDEDIR)
	cp logic.h        $(INCLUDEDIR)
	cp strutils.h     $(INCLUDEDIR)
	cp args/args.h    $(INCLUDEDIR)
	cp threads/job.h  $(INCLUDEDIR)/threads
	cp threads/pool.h $(INCLUDEDIR)/threads

compile: prepare $(OBJS)
	rm -f $(TARGET)
	$(AR) cr $(TARGET) $(OBJS)

$(OBJS): %.o : %.cc
	$(CXX) $(CXXFLAGS) $< -o $@

prepare:
	mkdir -p $(BUILDDIR)
	mkdir -p $(LIBDIR)
	mkdir -p $(BUILDDIR)/include
	mkdir -p $(INCLUDEDIR)
	mkdir -p $(INCLUDEDIR)/threads

clean:
	rm $(OBJS)

