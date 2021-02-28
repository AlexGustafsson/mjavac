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

bool analyze_class_semantics(SymbolTableView *view, const ProgramNode *program_node, const ClassDeclarationNode *class_node) {
  bool passed = true;

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

bool analyze_method_semantics(SymbolTableView *view, const ProgramNode *program_node, const MethodDeclarationNode *method_node) {
  bool passed = true;

  // TODO: Analyze each parameter (arrays etc.)

  // Analyze each statement
  for (const auto &statement_node : method_node->statements) {
    bool statement_passed = analyze_statement_semantics(view, program_node, statement_node);
    passed = passed && statement_passed;
  }

  return passed;
}

bool analyze_statement_semantics(SymbolTableView *view, const ProgramNode *program_node, const Node *statement_node) {
  // Analyze conditionals (conditional)
  const auto &conditional_node = dynamic_cast<const ConditionalNode *>(statement_node);
  if (conditional_node != nullptr) {
    return true;
  }

  // Analyze loops (loop)
  const auto &loop_node = dynamic_cast<const LoopNode*>(statement_node);
  if (loop_node != nullptr) {
    return true;
  }

  // Analyze an assignment   (expression = expression)
  const auto &binary_operation_node = dynamic_cast<const BinaryOperationNode *>(statement_node);
    if (binary_operation_node != nullptr) {
      // Analyze a variable assignment   (type identifier = expression)
      if (binary_operation_node->binary_operator == Operator::Assign) {
        //   bool passed = true;

        //   passed &= analyze_expression_semantics(view, program_node, binary_operation_node->right);

        //   const auto &left = dynamic_cast<const ValueNode *>(binary_operation_node->left);
        //   if (left->type == ValueNode::Identifier) {
        //     Symbol *variable_symbol = view->get_symbol_by_name(left->identifier_value);
        //     if (variable_symbol == nullptr) {
        //       // Not defined in this scope
        //       program_node->source->print_line_error(std::cerr, binary_operation_node->location.start_line, binary_operation_node->location.start_column, "assignment to non-defined variable");
        //       passed = false;
        //     } else {
        //       debug_out << "warning: unhandled assign " << std::setbase(16) << binary_operation_node->get_id() << std::endl;
        //     }
        //   } else if (left->type == ValueNode::Object) {
        //     debug_out << "warning: unhandled assign using object " << std::setbase(16) << binary_operation_node->get_id() << std::endl;
        //   } else {
        //     program_node->source->print_line_error(std::cerr, binary_operation_node->location.start_line, binary_operation_node->location.start_column, "assignment to non-variable");
        //     passed = false;
        //   }
        return true;
      } else {
        program_node->source->print_line_warning(std::cerr, binary_operation_node->location.start_line, binary_operation_node->location.start_column, "unused result from operation");
        return analyze_expression_semantics(view, program_node, binary_operation_node);
      }
    }

    // Analyze a variable declaration (type identifier)
    const auto &variable_node = dynamic_cast<const VariableNode *>(statement_node);
    if (variable_node != nullptr) {
      bool passed = true;

      // Error on array declaration of type other than int
      if (variable_node->type->is_array && variable_node->type->type.compare("int") != 0) {
        program_node->source->print_line_error(std::cerr, variable_node->location.start_line, variable_node->location.start_column, "array declaration of type other than int");
        passed = false;
      }

      // Error on duplicate variables
      if (view->count_symbols_by_name(variable_node->identifier) > 1) {
        program_node->source->print_line_error(std::cerr, variable_node->location.start_line, variable_node->location.start_column, "duplicate variable declaration");
        passed = false;
      }

      return passed;
    }

    // Analyze a return
    const auto &return_node = dynamic_cast<const ReturnNode *>(statement_node);
    if (return_node != nullptr) {
      return true;
    }

    // Analyze a basic expression
    return analyze_expression_semantics(view, program_node, statement_node);
}

bool analyze_expression_semantics(SymbolTableView *view, const ProgramNode *program_node, const Node *expression_node) {
  // Analyze binary expression (expression operator expression)
  // Analyze a single valie (value)
  // Analyze a binary expression (value [ expression ])
  // Analyze a unary operator unaryop expression
  // Analyze an array initialization node (new type [expression])
  // Analyze a class initialization node (new identiifer ())
  // Analyze method call (expression (parameter list))
  // Analyze a method call (expression ())
  // Analyze a subscript (expression [ expression ])
  // Analyze a dot access expression . identifier

  const auto &binary_operation_node = dynamic_cast<const BinaryOperationNode *>(expression_node);
  if (binary_operation_node != nullptr) {
    Symbol *left = view->symbol_table->get_symbol(binary_operation_node->left->get_id());
    Symbol *right = view->symbol_table->get_symbol(binary_operation_node->right->get_id());
    if (left == nullptr)
      debug_out << "warning: missing symbols for left binary operand " << std::setbase(16) << binary_operation_node->left->get_id() << " for operator " << std::setbase(16) << binary_operation_node->get_id() << std::endl;
    if (right == nullptr)
      debug_out << "warning: missing symbols for right binary operand " << std::setbase(16) << binary_operation_node->right->get_id() << " for operator " << std::setbase(16) << binary_operation_node->get_id() << std::endl;

    // TODO: This along with the logs above should be unnecessary once the symbol generator is fully implemented
    if (left == nullptr || right == nullptr)
      return false;

    if (binary_operation_node->binary_operator == Operator::Plus || binary_operation_node->binary_operator == Operator::Minus || binary_operation_node->binary_operator == Operator::Multiplication) {
      bool passed = true;
      passed &= analyze_expression_semantics(view, program_node, binary_operation_node->left);
      passed &= analyze_expression_semantics(view, program_node, binary_operation_node->right);

      if ((left->traits & SymbolTrait::IntLike) != (right->traits & SymbolTrait::IntLike)) {
        program_node->source->print_line_error(std::cerr, binary_operation_node->location.start_line, binary_operation_node->location.start_column, "unexpected operation between two values of different types");
        passed = false;
      }

      return passed;
    } else {
      debug_out << "warning: unhandled binary operator " << std::setbase(16) << binary_operation_node->get_id() << std::endl;
      return true;
    }
  }

  debug_out << "warning: unhandled expression semantics " << std::setbase(16) << expression_node->get_id() << std::endl;
  return true;
}
