# mrt c++ lib

export CXX      := g++
export AR       := ar
export CFLAGS   := -c -I./
export CXXFLAGS :=  $(CFLAGS) -std=c++17

NAME            := mrt
LIBNAME         := lib$(NAME).a
OBJS            := args/args.o threads/job.o threads/pool.o strutils.o

.PHONY: all

all: prepare compile clean

compile: $(OBJS)
	rm -f bin/$(LIBNAME)
	$(AR) cr bin/$(LIBNAME) $(OBJS)

$(OBJS): %.o : %.cc
	$(CXX) $(CXXFLAGS) $< -o $@

prepare:
	mkdir -p bin

clean:
	rm $(OBJS)

