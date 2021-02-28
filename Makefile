# mrt c++ lib

export CXX      := g++
export AR       := ar
export CXXFLAGS := -std=c++17 -c -I../

NAME            := mrt
LIBNAME         := lib$(NAME).a
OBJS            := $(addsuffix .o, $(basename $(wildcard */*.cc)))

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

