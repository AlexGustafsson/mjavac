#include <iostream>

#include "../debug.hpp"

#include "../symbol-generator.hpp"

void generate_symbols_for_method(SymbolTable *symbol_table, const MethodDeclarationNode *method_node, const ClassDeclarationNode *class_node) {
  // Add parameters
  for (const auto &parameter_node : method_node->parameters)
    generate_symbols_for_variable(symbol_table, parameter_node, method_node);

  // Add statements
  for (const auto &statement_node : method_node->statements)
    generate_symbols_for_statement(symbol_table, statement_node, method_node);
}
