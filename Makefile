# Disable echoing of commands
MAKEFLAGS += --silent

OUTPUT_PATH=build/production

# Optimize the code and show all warnings (except unused parameters)
BUILD_FLAGS=-I src -I $(OUTPUT_PATH) -I/usr/local/opt/flex/include -O3 -Wall -Wextra -pedantic -Wno-unused-parameter -std=c++17

# Don't optimize, provide all warnings and build with clang's memory checks and support for GDB debugging
DEBUG_FLAGS=-I src -I $(OUTPUT_PATH) -Wall -Wextra -pedantic -Wno-unused-parameter -std=c++17 -fsanitize=address -fno-omit-frame-pointer -g -DDEBUG

FLEX_FLAGS=
BISON_FLAGS=-t

ifdef DEBUG
	OUTPUT_PATH=build/debug
	CXX=clang++
	CC=clang
	BUILD_FLAGS=$(DEBUG_FLAGS)
	FLEX_FLAGS=--debug
	BISON_FLAGS=-v -t --graph
endif

# Source code
source := $(shell find src -type f -name "*.cc")
headers := $(shell find src -type f -name "*.hpp")
objects := $(subst src,$(OUTPUT_PATH),$(source:.cc=.o))

.PHONY: build debug format analyze lint test package clean

# Build mjavac, default action
build: $(OUTPUT_PATH)/mjavac
	ln -s $(PWD)/build/production/mjavac $(PWD)/build/mjavac &> /dev/null || true
	ln -s $(PWD)/build/debug/mjavac $(PWD)/build/mjavac.debug &> /dev/null || true

# Build mjavac with extra debugging enabled
debug:
	DEBUG=true $(MAKE)

# Executable linking
# The repeat is there since it seems the target-specific overriding of OUTPUT_PATH
# will not occur in time for $(OUTPUT_PATH) to be used in the rule's output
$(OUTPUT_PATH)/mjavac: $(OUTPUT_PATH)/lexer.yy.o $(OUTPUT_PATH)/parser.tab.o $(objects)
	$(CXX) $(BUILD_FLAGS) -o $@ $(OUTPUT_PATH)/lexer.yy.o $(OUTPUT_PATH)/parser.tab.o $(objects)

# Source compilation
$(objects): $(OUTPUT_PATH)/%.o: src/%.cc src/%.hpp
	mkdir -p $(dir $@)
	$(CXX) $(BUILD_FLAGS) -c $< -o $@

# Compile the lexer
$(OUTPUT_PATH)/lexer.yy.o: $(OUTPUT_PATH)/parser.tab.hpp $(OUTPUT_PATH)/lexer.yy.cc
	$(CXX) $(BUILD_FLAGS) -Wno-unused-function -c $(OUTPUT_PATH)/lexer.yy.cc -o $(OUTPUT_PATH)/lexer.yy.o

# Generate the lexer
$(OUTPUT_PATH)/lexer.yy.cc: src/lexer.l
	mkdir -p $(OUTPUT_PATH)
	flex $(FLEX_FLAGS) --outfile $(OUTPUT_PATH)/lexer.yy.cc $<

# Compile the parser
$(OUTPUT_PATH)/parser.tab.o: $(OUTPUT_PATH)/parser.tab.cc
	$(CXX) $(BUILD_FLAGS) -c $(OUTPUT_PATH)/parser.tab.cc -o $(OUTPUT_PATH)/parser.tab.o

# Generate the parser
$(OUTPUT_PATH)/parser.tab.cc $(OUTPUT_PATH)/parser.tab.hpp: src/parser.yy
	mkdir -p $(OUTPUT_PATH)
	bison $(BISON_FLAGS) $< --output=$(OUTPUT_PATH)/parser.tab.cc --defines=$(OUTPUT_PATH)/parser.tab.hpp

# Create the compilation database for llvm tools
compile_commands.json: Makefile
	# compiledb is installed using: pip install compiledb
	compiledb -n make

# Format code according to .clang-format
format: compile_commands.json
	clang-format -style=file -i $(source) $(headers)

# Analyze code and produce a report using the llvm tool scan-build
analyze: compile_commands.json
	scan-build --force-analyze-debug-code --keep-going -o build/reports/static-analysis make

# Lint the code according to .clang-format
lint: compile_commands.json
	./ci/lint.sh $(source)

test: build/debug/mjavac
	./test/test.sh build/debug/mjavac

package:
	zip -r archive.zip README.md Makefile .clang-format test src ci

clean:
	rm -rf build/*
