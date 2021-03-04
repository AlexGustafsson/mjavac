#include <iostream>

#include "../debug.hpp"

#include "../semantics-analyzer.hpp"

bool analyze_program_semantics(const SymbolTable *symbol_table, const ProgramNode *program_node) {
  bool passed = true;

  // Create a view scoped to the program
  SymbolTableView *view = symbol_table->create_view();
  view->set_scope(program_node->get_id());

  // Warn if there are no classes defined
  if (view->count_symbols() == 0)
    program_node->source->print_line_warning(std::cerr, program_node->location.start_line, program_node->location.start_column, "no class definitions");

  for (const auto &class_node : program_node->declarations) {
    // Throw an error if there are duplicate classes
    if (view->count_symbols_by_name(class_node->identifier) > 1) {
      program_node->source->print_line_error(std::cerr, class_node->location.start_line, class_node->location.start_column, "duplicate class definition");
      passed = false;
    }

    view->set_scope(class_node->get_id());
    bool class_passed = analyze_class_semantics(view, program_node, class_node);
    passed = passed && class_passed;
    view->set_scope(program_node->get_id());
  }

  return passed;
}
