# Quickstart

This project holds modules which together build up mjavac - a MiniJava compiler (with extensions).

Each part is self-contained and reusable. For example, the parser may be used as a library to build syntax highlighters, interpreters and so on. The parser library is used by mjavac to build an AST for the source code.

The top-level project builds the complete compiler.

```sh
make build
./build/mjavac test/examples/factorial.java
```

## Parser

The simples way to build the parser is to simply run `make parser`.

To build the parser by itself one can run `make -C parser`. Note however, that the compiler flags represent a bare minimum and are supposed to be tuned, such as is done by the main project's Makefile. To compile the parser in its simplest form simply ensure that `$CXXFLAGS` contains at least `-std=c++17` like so: `CXXFLAGS=-std=c++17 make -C parser`. The project can be built in debug mode by setting the `DEBUG` environment variable like so: `DEBUG=true ...`.

Once built, the `parser/build` directory is populated with the following directories for the chosen build configuration (`production`, `debug`):

* `include`
* `lib`

These directories are to be referenced when building projects using the parser like so:

```sh
g++ my-compiler.cc -I parser/build/production/include -L parser/build/production/lib/mjavac -l mjavacparser
```

# Development

Make sure you meet the following prerequisites:

Building:
* `$CXX` refers to a modern C++ compiler:
  * `g++` 10 or newer (`brew install gcc` on macOS)
  * `clang++` 10 or newer (`brew install llvm` on macOS)
* `$AR` refers to an appropriate version of `ar` for the same toolchain

Developing:
* `clang` 10 or newer is installed
* `scan-build` refers to version 7 or newer which comes with `clang`
* `clang-format` refers to version 7 or newer which comes with `clang`

## General code operations

```sh
# Lint
make lint

# Perform static analysis
make analyze
# The report is now available in build/reports/static-analysis

# Format the code
make format
```
