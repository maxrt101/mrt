# mrt c++ lib

CXX             ?= g++
AR              ?= ar
CFLAGS          += -c -I$(PREFIX)/include
CXXFLAGS        +=  $(CFLAGS) -std=c++17

LIBDIR          := $(PREFIX)/lib
INCLUDEDIR      := $(PREFIX)/include/mrt

TARGET          := $(LIBDIR)/libmrt.a
OBJS            := args/args.o test/test.o test/framework.o strutils.o

ifeq ("$(DEBUG)","1")
CFLAGS += -g3 -DDEBUG
endif

.PHONY: build

build: install-headers $(OBJS)
	rm -f $(TARGET)
	$(AR) cr $(TARGET) $(OBJS)

$(OBJS): %.o : %.cc
	$(info Building $@)
	$(CXX) $(CXXFLAGS) $< -o $@

install-headers: prepare
	cp *.h         $(INCLUDEDIR)
	cp args/*.h    $(INCLUDEDIR)/args
	cp test/*.h    $(INCLUDEDIR)/test
	cp threads/*.h $(INCLUDEDIR)/threads

prepare:
	mkdir -p $(PREFIX)
	mkdir -p $(LIBDIR)
	mkdir -p $(PREFIX)/include
	mkdir -p $(INCLUDEDIR)
	mkdir -p $(INCLUDEDIR)/args
	mkdir -p $(INCLUDEDIR)/test
	mkdir -p $(INCLUDEDIR)/threads

clean:
	rm $(OBJS)

