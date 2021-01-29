# Disable echoing of commands
MAKEFLAGS += --silent

# Optimize the code and show all warnings (except unused parameters)
BUILD_FLAGS=-O3 -Wall -Wextra -pedantic -Wno-unused-parameter $(BUILD_VARIABLES)

# Don't optimize, provide all warnings and build with clang's memory checks and support for GDB debugging
DEBUG_FLAGS=-Wall -Wextra -pedantic -Wno-unused-parameter -fsanitize=address -fno-omit-frame-pointer -g $(BUILD_VARIABLES) -DDEBUG

# Source code
source := $(shell find src -type f -name "*.c")
headers := $(shell find src -type f -name "*.h")
objects := $(subst src,build,$(source:.c=.o))

filesToFormat := $(source) $(sourceHeaders)

.PHONY: build debug format analyze lint test package clean

# Build mjavac, default action
build: build/mjavac

# Build mjavac with extra debugging enabled
debug: CC=clang
debug: build/mjavac.debug

# Executable linking
build/mjavac: $(objects) build/lexer.yy.o
	$(CC) $(BUILD_FLAGS) -o $@ $(objects) build/lexer.yy.o

build/mjavac.debug: $(objects) build/lexer.yy.debug.o
	$(CC) $(DEBUG_FLAGS) -o $@ $(objects) build/lexer.yy.debug.o

# Source compilation
$(objects): build/%.o: src/%.c src/%.h
	mkdir -p $(dir $@)
	$(CC) $(BUILD_FLAGS) -c $< -o $@

build/lexer.yy.o: build/lexer.yy.c
	$(CC) $(BUILD_FLAGS) -Wno-unused-function -Wno-unneeded-internal-declaration -c $< -o $@

build/lexer.yy.debug.o: build/lexer.yy.debug.c
	$(CC) $(BUILD_FLAGS) -Wno-unused-function -Wno-unneeded-internal-declaration -c $< -o $@

build/lexer.yy.c: src/lexer.flex
	mkdir -p build
	flex -o $@ $<

build/lexer.yy.debug.c: src/lexer.flex
	mkdir -p build
	flex --debug --outfile $@ $<

# Create the compilation database for llvm tools
compile_commands.json: Makefile
	# compiledb is installed using: pip install compiledb
	compiledb -n make

# Format code according to .clang-format
format: compile_commands.json
	clang-format -i -style=file $(filesToFormat)

# Analyze code and produce a report using the llvm tool scan-build
analyze: compile_commands.json
	scan-build --force-analyze-debug-code --keep-going -o build/reports/static-analysis make

# Lint the code according to .clang-format
lint: compile_commands.json
	./ci/lint.sh $(source)

test: build/mjavac
	./test/test.sh build/mjavac

package:
	zip -r archive.zip README.md Makefile .clang-format test src ci

clean:
	rm -rf build/*
