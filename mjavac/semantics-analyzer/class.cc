#include <iostream>

#include "../debug.hpp"

#include "../semantics-analyzer.hpp"

bool analyze_class_semantics(SymbolTableView *view, const ProgramNode *program_node, const ClassDeclarationNode *class_node) {
  bool passed = true;

  // Analyze the extension (if there is any)
  if (!class_node->extended_identifier.empty()) {
    if (view->count_symbols_by_name(class_node->extended_identifier) == 0) {
      program_node->source->print_line_error(std::cerr, class_node->location.start_line, class_node->location.start_column, "the extended class does not exist");
      passed = false;
    }
  }

  for (const auto &variable_node : class_node->variable_declarations) {
    // Throw an error if there are duplicate declarations
    if (view->count_symbols_by_name(variable_node->identifier) > 1) {
      program_node->source->print_line_error(std::cerr, variable_node->location.start_line, variable_node->location.start_column, "duplicate member definition");
      passed = false;
    }
  }

  for (const auto &variable_assignment : class_node->variable_assignments) {
    // Throw an error if there are duplicate declarations
    const VariableNode *variable_node = dynamic_cast<const VariableNode *>(variable_assignment);
    if (variable_node == nullptr)
      continue; // TODO: Print error
    if (view->count_symbols_by_name(variable_node->identifier) > 1) {
      program_node->source->print_line_error(std::cerr, variable_node->location.start_line, variable_node->location.start_column, "duplicate member definition");
      passed = false;
    }

    // TODO: Throw an error if the type assigned is incorrect.

    // Analyze the assigned expression
    bool expression_passed = analyze_expression_semantics(view, program_node, variable_assignment->right);
    passed = passed && expression_passed;
  }

  for (const auto &method_node : class_node->method_declarations) {
    // Throw an error if there are duplicate declarations
    if (view->count_symbols_by_name(method_node->identifier) > 1) {
      program_node->source->print_line_error(std::cerr, method_node->location.start_line, method_node->location.start_column, "duplicate method definition");
      passed = false;
    }

    view->set_scope(method_node->get_id());
    bool method_passed = analyze_method_semantics(view, program_node, method_node);
    passed = passed && method_passed;
    view->set_scope(class_node->get_id());
  }

  return passed;
}
