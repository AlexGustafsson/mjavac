# Disable echoing of commands
MAKEFLAGS += --silent

OUTPUT_PATH=build/production

# Use PARSER_PATH to control where the mjavac parser library is found.
# Defaults to the output created by the parser project. Expects
# the directory where both lib and include resides
REAL_PARSER_PATH=$(PARSER_PATH)
ifndef PARSER_PATH
REAL_PARSER_PATH=../parser/build/production
endif

# Use VM_PATH to control where the mjavac virtual machine library is found.
# Defaults to the output created by the virtual machine project. Expects
# the directory where both lib and include resides
REAL_VM_PATH=$(VM_PATH)
ifndef VM_PATH
REAL_VM_PATH=../virtual-machine/build/production
endif

ifdef DEBUG
OUTPUT_PATH=build/debug
ifndef PARSER_PATH
REAL_PARSER_PATH=../parser/build/debug
endif
ifndef VM_PATH
REAL_VM_PATH=../virtual-machine/build/debug
endif
endif

GRAPHVIZ_SUPPORT ?= true
ifeq ($(GRAPHVIZ_SUPPORT),true)
CPPFLAGS += -DGRAPHVIZ_SUPPORT
LDLIBS += -l gvc -l cgraph
endif

CPPFLAGS += -I $(REAL_PARSER_PATH)/include -I $(REAL_VM_PATH)/include
LDFLAGS += -L $(REAL_PARSER_PATH)/lib/mjavac -L $(REAL_VM_PATH)/lib/mjavac
LDLIBS += -l mjavacparser -l mjavacvm

source := $(shell find * -type f -name "*.cc" -not -path "build/*")
objects := $(addprefix $(OUTPUT_PATH)/,$(source:.cc=.o))
headers := $(shell find * -type f -name "*.hpp" -not -path "build/*")

.PHONY: build clean

build: $(OUTPUT_PATH)/mjavac

$(OUTPUT_PATH)/mjavac: $(objects) $(headers) $(REAL_PARSER_PATH)/lib/mjavac/libmjavacparser.a $(REAL_VM_PATH)/lib/mjavac/libmjavacvm.a
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $(objects) $(LDLIBS)

# Compile source
$(objects): $(OUTPUT_PATH)/%.o: %.cc $(wildcard %.hpp)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm -r build &> /dev/null || true
