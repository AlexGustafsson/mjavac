#include <iostream>

#include "debug.hpp"

#include "semantics-analyzer.hpp"

bool analyze_program_semantics(const SymbolTable *symbol_table, const ProgramNode *program_node) {
  bool passed = true;

  // Create a view scoped to the program
  SymbolTableView *view = symbol_table->create_view();
  view->set_scope(program_node->get_id());

  // Warn if there are no classes defined
  if (view->count_symbols() == 0)
    std::cerr << "\033[1m" << program_node->file_name << ":" << program_node->location.start_line << ":" << program_node->location.start_column << ":\033[33m warning:\033[0m no class definitions" << std::endl;

  for (const auto &class_node : program_node->declarations) {
    // Throw an error if there are duplicate classes
    if (view->count_symbols_by_name(class_node->identifier) > 1) {
      std::cerr << "\033[1m" << program_node->file_name << ":" << class_node->location.start_line << ":" << class_node->location.start_column << ":\033[31m error:\033[0m duplicate class definition '" << class_node->identifier << "'" << std::endl;
      passed = false;
    }

    view->set_scope(class_node->get_id());
    bool class_passed = analyze_class_semantics(view, program_node, class_node);
    passed = passed && class_passed;
    view->set_scope(program_node->get_id());
  }

  return passed;
}

bool analyze_class_semantics(SymbolTableView *view, const ProgramNode *program_node, const ClassDeclarationNode *class_node) {
  bool passed = true;

  for (const auto &variable_node : class_node->variable_declarations) {
    // Throw an error if there are duplicate declarations
    if (view->count_symbols_by_name(variable_node->identifier) > 1) {
      std::cerr << "\033[1m" << program_node->file_name << ":" << variable_node->location.start_line << ":" << variable_node->location.start_column << ":\033[31m error:\033[0m duplicate variable definition'" << variable_node->identifier << "'" << std::endl;
      passed = false;
    }
  }

  for (const auto &method_node : class_node->method_declarations) {
    // Throw an error if there are duplicate declarations
    if (view->count_symbols_by_name(method_node->identifier) > 1) {
      std::cerr << "\033[1m" << program_node->file_name << ":" << method_node->location.start_line << ":" << method_node->location.start_column << ":\033[31m error:\033[0m duplicate method definition '" << method_node->identifier << "'" << std::endl;
      passed = false;
    }

    view->set_scope(method_node->get_id());
    bool method_passed = analyze_method_semantics(view, program_node, method_node);
    passed = passed && method_passed;
    view->set_scope(class_node->get_id());
  }

  return passed;
}

bool analyze_method_semantics(SymbolTableView *view, const ProgramNode *program_node, const MethodDeclarationNode *method_node) {
  bool passed = true;

  for (const auto &statement_node : method_node->statements) {
    bool statement_passed = analyze_statement_semantics(view, program_node, statement_node);
    passed = passed && statement_passed;
  }

  return passed;
}

bool analyze_statement_semantics(SymbolTableView *view, const ProgramNode *program_node, const Node *statement_node) {
  const auto &binary_operation_node = dynamic_cast<const BinaryOperationNode *>(statement_node);
  if (binary_operation_node != nullptr) {
    if (binary_operation_node->binary_operator != Operator::Assign)
      std::cerr << "\033[1m" << program_node->file_name << ":" << binary_operation_node->location.start_line << ":" << binary_operation_node->location.start_column << ":\033[33m warning:\033[0m unused result from operation" << std::endl;
    return analyze_expression_semantics(view, program_node, binary_operation_node);
  }

  debug_out << "warning: unhandled statement semantics " << std::setbase(16) << statement_node->get_id() << std::endl;
  return true;
}

bool analyze_expression_semantics(SymbolTableView *view, const ProgramNode *program_node, const Node *expression_node) {
  const auto &binary_operation_node = dynamic_cast<const BinaryOperationNode *>(expression_node);
  if (binary_operation_node != nullptr) {
    Symbol *left = view->symbol_table->get_symbol(binary_operation_node->left->get_id());
    Symbol *right = view->symbol_table->get_symbol(binary_operation_node->right->get_id());
    if (left == nullptr)
      debug_out << "warning: missing symbols for left binary operand " << std::setbase(16) << binary_operation_node->left->get_id() << " for operator " << std::setbase(16) << binary_operation_node->get_id() << std::endl;
    if (right == nullptr)
      debug_out << "warning: missing symbols for right binary operand " << std::setbase(16) << binary_operation_node->right->get_id() << " for operator " << std::setbase(16) << binary_operation_node->get_id() << std::endl;

    if (binary_operation_node->binary_operator == Operator::Plus || binary_operation_node->binary_operator == Operator::Minus || binary_operation_node->binary_operator == Operator::Multiplication) {
      analyze_expression_semantics(view, program_node, binary_operation_node->left);
      analyze_expression_semantics(view, program_node, binary_operation_node->right);

      if ((left->traits & SymbolTrait::IntLike) != (right->traits & SymbolTrait::IntLike)) {
        std::cerr << "\033[1m" << program_node->file_name << ":" << binary_operation_node->location.start_line << ":" << binary_operation_node->location.start_column << ":\033[31m error:\033[0m unexpected operation between two values of different types" << std::endl;
        return false;
      }
    } else if (binary_operation_node->binary_operator == Operator::Assign) {
      analyze_expression_semantics(view, program_node, binary_operation_node->right);
      debug_out << "warning: unhandled assign " << std::setbase(16) << binary_operation_node->get_id() << std::endl;
    } else {
      debug_out << "warning: unhandled binary operator " << std::setbase(16) << binary_operation_node->get_id() << std::endl;
      return true;
    }
  }

  debug_out << "warning: unhandled expression semantics " << std::setbase(16) << expression_node->get_id() << std::endl;
  return true;
}
