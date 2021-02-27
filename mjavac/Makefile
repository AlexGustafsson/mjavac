# Disable echoing of commands
MAKEFLAGS += --silent

OUTPUT_PATH=build/production

GRAPHVIZ_SUPPORT ?= true
ifeq ($(GRAPHVIZ_SUPPORT),true)
CPPFLAGS += -DGRAPHVIZ_SUPPORT
endif

# Use PARSER_PATH to control where the mjavac parser library is found.
# Defaults to the output created by the parser project. Expects
# the directory where both lib and include resides
REAL_PARSER_PATH=$(PARSER_PATH)
ifndef PARSER_PATH
REAL_PARSER_PATH=../parser/build/production
endif

ifdef DEBUG
OUTPUT_PATH=build/debug
ifndef PARSER_PATH
REAL_PARSER_PATH=../parser/build/debug
endif
endif

CPPFLAGS += -I $(REAL_PARSER_PATH)/include
LDFLAGS += -L $(REAL_PARSER_PATH)/lib/mjavac

.PHONY: build clean

build: $(OUTPUT_PATH)/mjavac

$(OUTPUT_PATH)/mjavac: main.cc main.hpp symbol-table.cc symbol-table.hpp symbol-generator.cc symbol-generator.hpp semantics-analyzer.cc semantics-analyzer.hpp debug.hpp $(REAL_PARSER_PATH)/lib/mjavac/libmjavacparser.a
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ main.cc symbol-table.cc symbol-generator.cc semantics-analyzer.cc -l gvc -l cgraph -l mjavacparser

clean:
	rm -r build &> /dev/null || true
