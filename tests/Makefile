# mrt/tests

TOPDIR   ?= ..
PREFIX   ?= $(TOPDIR)/build

CXX      := g++

CXXFLAGS += -std=c++17 -I$(PREFIX)/include
LDFLAGS  += -L$(PREFIX)/lib -lmrt

SRC      := test_args.cc test_optional.cc test_container_utils.cc test_future.cc \
            test_executor.cc test_delayed_executor.cc test_interval_executor.cc \
            test_pool.cc

.PHONY: test

test: build
	for test in $(PREFIX)/bin/*; do \
		echo "Running $$test"; \
		$$test; \
		echo; \
	done

build:
	mkdir -p $(PREFIX)/bin
	for test in $(SRC); do \
		if [ ! -f "$(PREFIX)/bin/$${test%.*}" ]; then \
			echo "Building $$test"; \
			$(CXX) $(CXXFLAGS) $(LDFLAGS) $$test -o $(PREFIX)/bin/$${test%.*}; \
		fi; \
	done

$(V).SILENT:

