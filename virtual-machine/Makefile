# Disable echoing of commands
MAKEFLAGS += --silent

OUTPUT_PATH=build/production

FLEX_FLAGS=
BISON_FLAGS=-t

ifdef DEBUG
OUTPUT_PATH=build/debug
FLEX_FLAGS=--debug
BISON_FLAGS=-v -t --graph
endif

source := $(shell find * -type f -name "*.cc")
objects := $(addprefix $(OUTPUT_PATH)/,$(source:.cc=.o))
headers := $(shell find * -type f -name "*.hpp" -not -path "build/*")
includes := $(addprefix $(OUTPUT_PATH)/include/mjavac/,$(headers))

.PHONY: lib include clean

all: lib include

include: $(includes)

lib: $(OUTPUT_PATH)/lib/mjavac/libmjavacvm.a

# Build a static library for the parser
$(OUTPUT_PATH)/lib/mjavac/libmjavacvm.a: $(objects)
	mkdir -p $(dir $@)
ifeq ($(shell uname),Darwin)
	libtool -static -o $@ $^
else
	$(AR) rcs $@ $^
endif

# Copy headers
$(includes): $(OUTPUT_PATH)/include/mjavac/%.hpp: %.hpp
	mkdir -p $(dir $@)
	cp $< $@

# Compile ast
$(objects): $(OUTPUT_PATH)/%.o: %.cc %.hpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm -r build &> /dev/null || true
