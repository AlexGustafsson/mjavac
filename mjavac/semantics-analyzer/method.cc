#include <iostream>

#include "../debug.hpp"

#include "../semantics-analyzer.hpp"

bool analyze_method_semantics(SymbolTableView *view, const ProgramNode *program_node, const MethodDeclarationNode *method_node) {
  bool passed = true;

  // TODO: Analyze each parameter (arrays etc.)

  // Analyze each statement
  for (const auto &statement_node : method_node->statements)
    passed &= analyze_statement_semantics(view, program_node, method_node, statement_node);
  return passed;
}
