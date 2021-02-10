# Quickstart

```sh
make build
./build/mjavac test/examples/factorial.java
```

# Development

Make sure you meet the following prerequisites:

Building:
* `$CXX` refers to a modern compiler:
  * `gcc` 10 or newer (`brew install gcc` on macOS)
  * `clang` 10 or newer (`brew install llvm` on macOS)

Developing:
* `clang` 10 or newer is installed
* `scan-build` refers to version 7 or newer which comes with `clang`
* `clang-format` refers to version 7 or newer which comes with `clang`

```sh
# Lint
make lint

# Perform static analysis
make analyze
# The report is now available in build/reports/static-analysis

# Format the code
make format

# Run tests
make test

# Build and run a debugging build (memory analyzer and GDB debugging enabled)
make debug
ASAN_OPTIONS=detect_leaks=1 ./build/mjavac.debug

# Build and run a release build
make build
./build/mjavac
```
