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

bool analyze_method_semantics(SymbolTableView *view, const ProgramNode *program_node, const MethodDeclarationNode *method_node) {
  bool passed = true;

  // TODO: Analyze each parameter (arrays etc.)

  // Analyze each statement
  for (const auto &statement_node : method_node->statements)
    passed &= analyze_statement_semantics(view, program_node, method_node, statement_node);
  return passed;
}

bool analyze_statement_semantics(SymbolTableView *view, const ProgramNode *program_node, const MethodDeclarationNode *method_node, const Node *statement_node) {
  // Analyze conditionals (conditional)
  const auto &conditional_node = dynamic_cast<const ConditionalNode *>(statement_node);
  if (conditional_node != nullptr) {
    return true;
  }

  // Analyze loops (loop)
  const auto &loop_node = dynamic_cast<const LoopNode *>(statement_node);
  if (loop_node != nullptr) {
    return true;
  }

  // Analyze an assignment   (expression = expression)
  const auto &binary_operation_node = dynamic_cast<const BinaryOperationNode *>(statement_node);
  if (binary_operation_node != nullptr) {
    // Analyze a variable assignment
    if (binary_operation_node->binary_operator == Operator::Assign) {
      bool passed = true;

      // Analyze the variable declaration
      const auto &variable_node = dynamic_cast<const VariableNode *>(binary_operation_node->left);
      if (variable_node != nullptr) {
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
      }

      // Analyze the expression
      passed &= analyze_expression_semantics(view, program_node, binary_operation_node->right);

      //   passed &= analyze_expression_semantics(view, program_node, binary_operation_node->right);

      //   const auto &left = dynamic_cast<const ValueNode *>(binary_operation_node->left);
      //   if (left->type == ValueNode::Identifier) {
      //     Symbol *variable_symbol = view->get_symbol_by_name(left->identifier_value);
      //     if (variable_symbol == nullptr) {
      //       // Not defined in this scope
      //       program_node->source->print_line_error(std::cerr, binary_operation_node->location.start_line, binary_operation_node->location.start_column, "assignment to non-defined variable");
      //       passed = false;
      //     } else {
      //       debug_out << "warning: unhandled assign " << std::setbase(16) << binary_operation_node->get_id() << std::setbase(10) << std::endl;
      //     }
      //   } else if (left->type == ValueNode::Object) {
      //     debug_out << "warning: unhandled assign using object " << std::setbase(16) << binary_operation_node->get_id() << std::setbase(10) << std::endl;
      //   } else {
      //     program_node->source->print_line_error(std::cerr, binary_operation_node->location.start_line, binary_operation_node->location.start_column, "assignment to non-variable");
      //     passed = false;
      //   }
      return passed;
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
    bool passed = true;

    if (return_node->value == nullptr) {
      if (method_node->type->type.compare("void") != 0) {
        program_node->source->print_line_error(std::cerr, return_node->location.start_line, return_node->location.start_column, "must return a value in non-void method");
        passed = false;
      }
    } else {
      if (method_node->type->type.compare("void") == 0) {
        program_node->source->print_line_error(std::cerr, return_node->location.start_line, return_node->location.start_column, "must not return a value in void method");
        passed = false;
      } else {
        Symbol *value = view->symbol_table->get_symbol(return_node->value->get_id());
        if (value == nullptr) {
          debug_out << "warning: missing symbol for return value " << std::setbase(16) << return_node->value->get_id() << std::setbase(10) << " for return " << std::setbase(16) << return_node->get_id() << std::setbase(10) << std::endl;
        } else {
          int flags = view->resolve_flags(value);
          bool is_valid_int = method_node->type->type.compare("int") == 0 && flags & SymbolTrait::IntLike;
          bool is_valid_boolean = method_node->type->type.compare("boolean") == 0 && flags & SymbolTrait::BooleanLike;
          if (!is_valid_int && !is_valid_boolean) {
            // TODO: Deactivate for now.
            // The handling of objects does not really work. The idea was to handle some recursive lookup via
            // "behaves like" which then resolves the value within the scope. This is not properly implemented
            // for "dot" operations - object accessors such as "this.works". Furthermore,
            // "this" doesn't work at all currently. How should it be handled? Right now the closest would be
            // to use the recursive lookup and when "this" is seen, use some local scope? Another way would
            // be to put it on the parser with a new "this accessors"...
            // Or, YAGNI and just hard-code stuff for library methods :(
            // program_node->source->print_line_error(std::cerr, return_node->value->location.start_line, return_node->value->location.start_column, "incorrect return type");
            // passed = false;
          }
        }

        passed &= analyze_expression_semantics(view, program_node, return_node->value);
      }
    }

    return passed;
  }

  // Analyze a basic expression
  return analyze_expression_semantics(view, program_node, statement_node);
}

bool analyze_expression_semantics(SymbolTableView *view, const ProgramNode *program_node, const Node *expression_node) {
  // Analyze an array initialization node (new type [expression])
  // Analyze a class initialization node (new identiifer ())
  // Analyze method call (expression (parameter list))
  // Analyze a method call (expression ())
  // Analyze a subscript (expression [ expression ])
  // Analyze a dot access expression . identifier

  // Analyze binary operations
  const auto &binary_operation_node = dynamic_cast<const BinaryOperationNode *>(expression_node);
  if (binary_operation_node != nullptr) {
    Symbol *left = view->symbol_table->get_symbol(binary_operation_node->left->get_id());
    Symbol *right = view->symbol_table->get_symbol(binary_operation_node->right->get_id());
    if (left == nullptr)
      debug_out << "warning: missing symbols for left binary operand " << std::setbase(16) << binary_operation_node->left->get_id() << std::setbase(10) << " for operator " << std::setbase(16) << binary_operation_node->get_id() << std::setbase(10) << std::endl;
    if (right == nullptr)
      debug_out << "warning: missing symbols for right binary operand " << std::setbase(16) << binary_operation_node->right->get_id() << std::setbase(10) << " for operator " << std::setbase(16) << binary_operation_node->get_id() << std::setbase(10) << std::endl;

    // TODO: This along with the logs above should be unnecessary once the symbol generator is fully implemented
    if (left == nullptr || right == nullptr)
      return false;

    if (binary_operation_node->binary_operator == Operator::Plus || binary_operation_node->binary_operator == Operator::Minus || binary_operation_node->binary_operator == Operator::Multiplication || binary_operation_node->binary_operator == Operator::Division || binary_operation_node->binary_operator == Operator::LessThan || binary_operation_node->binary_operator == Operator::LessThanOrEqual || binary_operation_node->binary_operator == Operator::GreaterThan || binary_operation_node->binary_operator == Operator::GreaterThanOrEqual || binary_operation_node->binary_operator == Operator::Equal || binary_operation_node->binary_operator == NotEqual) {
      bool passed = true;
      passed &= analyze_expression_semantics(view, program_node, binary_operation_node->left);
      passed &= analyze_expression_semantics(view, program_node, binary_operation_node->right);

      bool operands_are_int_like = (view->resolve_flags(left) & SymbolTrait::IntLike) & (view->resolve_flags(right) & SymbolTrait::IntLike);
      if (!operands_are_int_like) {
        program_node->source->print_line_error(std::cerr, binary_operation_node->location.start_line, binary_operation_node->location.start_column, "unexpected operation between non-integer values");
        passed = false;
      }

      return passed;
    } else if (binary_operation_node->binary_operator == Operator::And || binary_operation_node->binary_operator == Operator::Or) {
      bool passed = true;
      passed &= analyze_expression_semantics(view, program_node, binary_operation_node->left);
      passed &= analyze_expression_semantics(view, program_node, binary_operation_node->right);

      bool operands_are_boolean_like = (view->resolve_flags(left) & SymbolTrait::BooleanLike) & (view->resolve_flags(right) & SymbolTrait::BooleanLike);
      if (!operands_are_boolean_like) {
        program_node->source->print_line_error(std::cerr, binary_operation_node->location.start_line, binary_operation_node->location.start_column, "unexpected logical operation between non-boolean values");
        passed = false;
      }

      return passed;
    } else if (binary_operation_node->binary_operator == Operator::Dot) {
      return true;
    } else if (binary_operation_node->binary_operator == Operator::Subscript) {
      bool passed = true;

      bool left_is_int_array = view->resolve_flags(left) & (SymbolTrait::Subscriptable | SymbolTrait::IntLike);
      if (!left_is_int_array) {
        program_node->source->print_line_error(std::cerr, binary_operation_node->left->location.start_line, binary_operation_node->left->location.start_column, "expression is not subscriptable");
        passed = false;
      }

      bool right_is_int = view->resolve_flags(right) & SymbolTrait::IntLike;
      if (!right_is_int) {
        program_node->source->print_line_error(std::cerr, binary_operation_node->right->location.start_line, binary_operation_node->right->location.start_column, "expression is not an integer");
        passed = false;
      }

      passed &= analyze_expression_semantics(view, program_node, binary_operation_node->right);
    } else {
      debug_out << "warning: unhandled binary operator " << std::setbase(16) << binary_operation_node->get_id() << std::setbase(10) << std::endl;
      return true;
    }
  }

  // Analyze unary operations
  const auto &unary_operation_node = dynamic_cast<const UnaryOperationNode *>(expression_node);
  if (unary_operation_node != nullptr) {
    Symbol *operand = view->symbol_table->get_symbol(unary_operation_node->operand->get_id());

    // TODO: This should be unnecessary once the symbol generator is fully implemented
    if (operand == nullptr) {
      debug_out << "warning: missing symbols for unary operand " << std::setbase(16) << unary_operation_node->operand->get_id() << std::setbase(10) << " for operator " << std::setbase(16) << unary_operation_node->get_id() << std::setbase(10) << std::endl;
      return true;
    }

    if (unary_operation_node->unary_operator == Operator::Negate) {
      bool passed = true;

      bool operand_is_boolean = view->resolve_flags(operand) & SymbolTrait::BooleanLike;
      if (!operand_is_boolean) {
        program_node->source->print_line_error(std::cerr, unary_operation_node->operand->location.start_line, unary_operation_node->operand->location.start_column, "unary operand is not a boolean");
        passed = false;
      }

      passed &= analyze_expression_semantics(view, program_node, unary_operation_node->operand);
      return passed;
    } else if (unary_operation_node->unary_operator == Operator::Negative) {
      bool passed = true;

      bool operand_is_int = view->resolve_flags(operand) & SymbolTrait::IntLike;
      if (!operand_is_int) {
        program_node->source->print_line_error(std::cerr, unary_operation_node->operand->location.start_line, unary_operation_node->operand->location.start_column, "unary operand is not an integer");
        passed = false;
      }

      passed &= analyze_expression_semantics(view, program_node, unary_operation_node->operand);
      return passed;
    } else {
      debug_out << "warning: unhandled unary operator " << std::setbase(16) << unary_operation_node->get_id() << std::setbase(10) << std::endl;
      return true;
    }
  }

  const auto &array_initialization_node = dynamic_cast<const ArrayInitializationNode *>(expression_node);
  if (array_initialization_node != nullptr) {
    Symbol *initialization_expression = view->symbol_table->get_symbol(array_initialization_node->initialization_expression->get_id());

    // TODO: This should be unnecessary once the symbol generator is fully implemented
    if (initialization_expression == nullptr) {
      debug_out << "warning: missing symbols for initialization_expression " << std::setbase(16) << array_initialization_node->initialization_expression->get_id() << std::setbase(10) << " for initialization " << std::setbase(16) << array_initialization_node->get_id() << std::setbase(10) << std::endl;
      return true;
    }

    bool passed = true;

    if (array_initialization_node->type->type.compare("int") != 0) {
      program_node->source->print_line_error(std::cerr, array_initialization_node->location.start_line, array_initialization_node->location.start_column, "the array is not for integers");
      passed = false;
    }

    bool initialization_expression_is_int = view->resolve_flags(initialization_expression) & SymbolTrait::IntLike;
    if (!initialization_expression_is_int) {
      program_node->source->print_line_error(std::cerr, array_initialization_node->initialization_expression->location.start_line, array_initialization_node->initialization_expression->location.start_column, "the initialization expression is not an integer");
      passed = false;
    }

    passed &= analyze_expression_semantics(view, program_node, array_initialization_node->initialization_expression);

    return passed;
  }

  debug_out << "warning: unhandled expression semantics " << std::setbase(16) << expression_node->get_id() << std::setbase(10) << std::endl;
  return true;
  }
