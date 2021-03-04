#include <iostream>

#include "../debug.hpp"

#include "../semantics-analyzer.hpp"

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
