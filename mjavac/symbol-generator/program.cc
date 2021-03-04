#include <iostream>

#include "../debug.hpp"

#include "../symbol-generator.hpp"

void generate_symbols_for_program(SymbolTable *symbol_table, const ProgramNode *program_node) {
  // Add the program's symbol
  symbol_table->add_symbol(new Symbol(program_node, 0, SymbolTrait::None));

  // Add each class
  // There is some nasty bug here. If there are no declarations in the program (empty source)
  // then declarations will have zero elements, but .empty() returns false. Therefore
  // the loop below will still run once, yielding a nullptr for the class node.
  // According to C++ documentation, the range-based for loop should handle that case
  // but apparently it doesn't
  // Using built - in specs.COLLECT_GCC = gcc
  // COLLECT_LTO_WRAPPER = / usr / local / Cellar / gcc / 10.2.0_3 / libexec / gcc / x86_64 - apple - darwin20 / 10.2.0 / lto - wrapper Target : x86_64 - apple - darwin20 Configured with :../ configure-- build = x86_64 - apple - darwin20-- prefix = / usr / local / Cellar / gcc / 10.2.0_3 --libdir = / usr / local / Cellar / gcc / 10.2.0_3 / lib / gcc / 10 --disable - nls-- enable - checking = release-- enable - languages = c,
  // c++, objc, obj - c++, fortran-- program - suffix = -10 --with - gmp = / usr / local / opt / gmp-- with - mpfr = / usr / local / opt / mpfr-- with - mpc = / usr / local / opt / libmpc-- with - isl = / usr / local / opt / isl-- with - system - zlib-- with - pkgversion = 'Homebrew GCC 10.2.0_3' --with - bugurl = https : //github.com/Homebrew/homebrew-core/issues --disable-multilib --with-native-system-header-dir=/usr/include --with-sysroot=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk SED=/usr/bin/sed
  // Thread model : posix Supported LTO compression algorithms : zlib gcc version 10.2.0(Homebrew GCC 10.2.0_3)
  // This check works around that issue.
  if (program_node->declarations.size() == 0)
    return;
  for (const auto &class_node : program_node->declarations) {
    generate_symbols_for_class(symbol_table, class_node, program_node);
  }
}
