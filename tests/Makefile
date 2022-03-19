# mrt/tests

TOPDIR   ?= ..
PREFIX   ?= $(TOPDIR)/build
BUILD    := $(TOPDIR)/build

CXX      := g++
CXXFLAGS += -std=c++17 -I$(PREFIX)/include

TARGET   := $(BUILD)/bin/mrt_test
SRC      := test_args.cc \
            test_config.cc \
            test_container_utils.cc \
            test_executor.cc \
            test_future.cc \
            test_locked.cc \
            test_optional.cc \
            test_pool.cc
TESTS     := $(addprefix $(BUILD)/test/, $(addsuffix .so, $(basename $(notdir $(SRC)))))

.PHONY: test

test: build
	$(info [+] Running $(notdir $(TARGET)))
	$(TARGET) $(TESTS)

build: prepare
	for file in $(SRC); do \
		echo "[+] Building $${file%.*}"; \
		$(CXX) $(CXXFLAGS) -c $$file -o $(BUILD)/obj/$$(basename $${file%.*}).o; \
		$(CXX) $(CXXFLAGS) -shared $(BUILD)/obj/$$(basename $${file%.*}).o -o $(BUILD)/test/$$(basename $${file%.*}).so; \
	done
	$(info [+] Builing $(notdir $(TARGET)))
	$(CXX) $(CXXFLAGS) main.cc -o $(TARGET)

# $(CXX) $(LDFLAGS) -shared $(4) $(addprefix $(BUILD)/obj/$(3)/, $(1)) -o $(BUILD)/lib/$(2)

prepare:
	$(info [+] Creating folders)
	mkdir -p $(PREFIX)/bin
	mkdir -p $(PREFIX)/obj
	mkdir -p $(PREFIX)/test

$(V).SILENT:
