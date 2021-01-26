Make sure you meet the following prerequisites:

Building:
* `$CC` refers to a modern compiler such as :
  * `gcc` 10 (`brew install gcc` on macOS)
  * `clang` 11 (`brew install llvm` on macOS)

Developing:
* `clang` 7 or newer is installed
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

# Build and run a debugging build (memory analyzer and GDB debugging enabled)
make debug
ASAN_OPTIONS=detect_leaks=1 ./build/mjavac.debug

# Build and run a release build
make build
./build/mjavac
```
