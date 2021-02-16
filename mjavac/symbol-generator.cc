#include "symbol-generator.hpp"
#include <iostream>

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

void generate_symbols_for_class(SymbolTable *symbol_table, const ClassDeclarationNode *class_node, const ProgramNode *program_node) {
  // Add a symbol for the class
  symbol_table->add_symbol(new Symbol(class_node, class_node->identifier, program_node->get_id(), SymbolTrait::Accessible | SymbolTrait::Initializable));

  // Add symbols for variables
  for (const auto &variable_node : class_node->variable_declarations)
    generate_symbols_for_variable(symbol_table, variable_node, class_node);

  // Add symbols for methods
  for (const auto &method_node : class_node->method_declarations)
    generate_symbols_for_method(symbol_table, method_node, class_node);
}

void generate_symbols_for_variable(SymbolTable *symbol_table, const VariableNode *variable_node, const Node *scope_node) {
  // Only create symbols for variable declarations
  if (!variable_node->is_declaration)
    return;

  int traits = SymbolTrait::None;
  if (variable_node->is_array)
    traits |= SymbolTrait::Subscriptable;
  if (variable_node->type == "int")
    traits |= SymbolTrait::IntLike;
  symbol_table->add_symbol(new Symbol(variable_node, variable_node->identifier, scope_node->get_id(), traits));
}

void
generate_symbols_for_method(SymbolTable *symbol_table, const MethodDeclarationNode *method_node, const ClassDeclarationNode *class_node) {
  // Add a symbol for the method itself
  symbol_table->add_symbol(new Symbol(method_node, method_node->identifier, class_node->get_id(), SymbolTrait::Callable));

  // Add parameters
  for (const auto &parameter_node : method_node->parameters)
    generate_symbols_for_variable(symbol_table, parameter_node, method_node);

  // Add statements
  for (const auto &statement_node : method_node->statements)
    generate_symbols_for_statement(symbol_table, statement_node, method_node);
}

void generate_symbols_for_statement(SymbolTable *symbol_table, const Node *statement_node, const Node *scope_node) {
  // const auto &loop_node = static_cast<mjavac::nodes::LoopNode *>(statement_node);
  // if (loop_node != nullptr) {
  //   // Add statements
  //   // ...
  //   continue;
  // }

  // const auto &conditional_node = static_cast<const ConditionalNode *>(statement_node);
  // if (conditional_node != nullptr) {
  //   for (const auto& nested_statement_node : conditional_node->statements)
  //     generate_symbols_for_statement(symbol_table, nested_statement_node, conditional_node);
  //   return;
  // }

  const auto &variable_node = dynamic_cast<const VariableNode *>(statement_node);
  if (variable_node != nullptr) {
    generate_symbols_for_variable(symbol_table, variable_node, scope_node);
    return;
  }
}
