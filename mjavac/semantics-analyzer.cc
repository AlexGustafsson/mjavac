#include <iostream>

#include "semantics-analyzer.hpp"

bool analyze_program_semantics(const SymbolTable *symbol_table, const ProgramNode *program_node) {
  bool passed = true;

  // Create a view scoped to the program
  SymbolTableView *view = symbol_table->create_view();
  view->set_scope(program_node->get_id());

  // Warn if there are no classes defined
  if (view->count_symbols() == 0)
    std::cerr << "\033[1mmjavac: \033[33mwarning:\033[0m " << program_node->file_name << ": no class definitions"<< std::endl;

  for (const auto &class_node : program_node->declarations) {
    // Throw an error if there are duplicate classes
    if (view->count_symbols_by_name(class_node->identifier) > 1) {
      std::cerr << "\033[1mmjavac: \033[31merror:\033[0m " << program_node->file_name << ": duplicate class name" << std::endl;
      passed = false;
    }

    view->set_scope(class_node->get_id());
    bool class_passed = analyze_class_semantics(view, program_node, class_node);
    passed = passed && class_passed;
    view->set_scope(program_node->get_id());
  }

  return passed;
}

bool analyze_class_semantics(const SymbolTableView *view, const ProgramNode *program_node, const ClassDeclarationNode *class_node) {
  bool passed = true;

  for (const auto &variable_node : class_node->variable_declarations) {
    // Throw an error if there are duplicate declarations
    if (view->count_symbols_by_name(variable_node->identifier) > 1) {
      std::cerr << "\033[1mmjavac: \033[31merror:\033[0m " << program_node->file_name << ": duplicate variable name" << std::endl;
      passed = false;
    }
  }

  for (const auto &method_node : class_node->method_declarations) {
    // Throw an error if there are duplicate declarations
    if (view->count_symbols_by_name(method_node->identifier) > 1) {
      std::cerr << "\033[1mmjavac: \033[31merror:\033[0m " << program_node->file_name << ": duplicate method name" << std::endl;
      passed = false;
    }
  }

  return passed;
}
