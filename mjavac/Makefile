# Disable echoing of commands
MAKEFLAGS += --silent

OUTPUT_PATH=build/production

CXXFLAGS := $(CXXFLAGS) -I ../parser/build/production/include

ifdef DEBUG
	OUTPUT_PATH=build/debug
endif

.PHONY: build clean

build: $(OUTPUT_PATH)/mjavac

$(OUTPUT_PATH)/mjavac: main.cc main.hpp ../parser/build/production/lib/mjavac/libmjavacparser.a
	mkdir -p $(dir $@)
	$(CXX) -o $@ main.cc $(CXXFLAGS) -L ../parser/build/production/lib/mjavac -l mjavacparser

clean:
	rm -r build &> /dev/null || true
