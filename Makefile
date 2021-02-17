# Disable echoing of commands
MAKEFLAGS += --silent

OUTPUT_PATH=build/production

# Optimize the code and show all warnings (except unused parameters)
export CXXFLAGS := $(CXXFLAGS) -O3 -Wall -Wextra -pedantic -Wno-unused-parameter -std=c++17 -g

# Don't optimize, provide all warnings and build with clang's memory checks and support for GDB debugging
DEBUG_FLAGS=-Wall -Wextra -pedantic -Wno-unused-parameter -std=c++17 -fsanitize=address -fno-omit-frame-pointer -g -DDEBUG

FLEX_FLAGS=
BISON_FLAGS=-t

ifdef DEBUG
	OUTPUT_PATH=build/debug
	CXX=clang++
	CC=clang
	CXXFLAGS=$(DEBUG_FLAGS)
	FLEX_FLAGS=--debug
	BISON_FLAGS=-v -t --graph
endif

# Source code
source := $(shell find * -type f -name "*.cc" -not -path "*/build/*" -not -path "build/*")
headers := $(shell find * -type f -name "*.hpp" -not -path "*/build/*" -not -path "build/*")

.PHONY: build debug mjavac parser format analyze lint test package clean

# Build mjavac, default action
build: parser mjavac

# Build mjavac with extra debugging enabled
debug:
	DEBUG=true $(MAKE)

# Build the compiler project
mjavac:
	$(MAKE) -C mjavac
	mkdir -p build
	ln -s $(PWD)/mjavac/build/production/mjavac $(PWD)/build/mjavac &> /dev/null || true
	ln -s $(PWD)/mjavac/build/debug/mjavac $(PWD)/build/mjavac.debug &> /dev/null || true

# Build the parser project
parser:
	$(MAKE) -C parser

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

test: debug
	./test/test.sh build/mjavac.debug

package:
	zip -r archive.zip README.md Makefile .clang-format test src ci

clean:
	rm -rf build compile_commands.json &>/dev/null || true
	$(MAKE) -C parser clean
	$(MAKE) -C mjavac clean
