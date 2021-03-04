#include <iostream>

#include "../debug.hpp"

#include "../symbol-generator.hpp"

void generate_symbols_for_expression(SymbolTable *symbol_table, const Node *expression_node, const Node *scope_node) {
  const auto &value_node = dynamic_cast<const ValueNode *>(expression_node);
  if (value_node != nullptr) {
    if (value_node->type == ValueNode::Integer)
      symbol_table->add_symbol(new Symbol(value_node, scope_node->get_id(), SymbolTrait::IntLike));
    else if (value_node->type == ValueNode::Boolean)
      symbol_table->add_symbol(new Symbol(value_node, scope_node->get_id(), SymbolTrait::BooleanLike));
    else if (value_node->type == ValueNode::Identifier)
      symbol_table->add_symbol(new Symbol(value_node, scope_node->get_id(), SymbolTrait::BehavesLikeIdentifier, value_node->identifier_value));
    else
      debug_out << "warning: unhandled value type " << value_node->type << std::endl;
    // TODO: add further types
    return;
  }

  const auto &binary_operation_node = dynamic_cast<const BinaryOperationNode *>(expression_node);
  if (binary_operation_node != nullptr) {
    generate_symbols_for_expression(symbol_table, binary_operation_node->left, scope_node);
    generate_symbols_for_expression(symbol_table, binary_operation_node->right, scope_node);

    if (binary_operation_node->binary_operator == Operator::And || binary_operation_node->binary_operator == Operator::LessThan || binary_operation_node->binary_operator == Operator::LessThanOrEqual || binary_operation_node->binary_operator == Operator::GreaterThan || binary_operation_node->binary_operator == Operator::GreaterThanOrEqual || binary_operation_node->binary_operator == Operator::Equal || binary_operation_node->binary_operator == Operator::NotEqual || binary_operation_node->binary_operator == Operator::Or || binary_operation_node->binary_operator == Operator::Negate) {
      symbol_table->add_symbol(new Symbol(binary_operation_node, scope_node->get_id(), SymbolTrait::BooleanLike));
    } else if (binary_operation_node->binary_operator == Operator::Plus || binary_operation_node->binary_operator == Operator::Minus || binary_operation_node->binary_operator == Operator::Multiplication || binary_operation_node->binary_operator == Operator::Division || binary_operation_node->binary_operator == Operator::Negative) {
      symbol_table->add_symbol(new Symbol(binary_operation_node, scope_node->get_id(), SymbolTrait::IntLike));
    } else if (binary_operation_node->binary_operator == Operator::Dot) {
      const auto &value_node = dynamic_cast<const ValueNode *>(binary_operation_node->right);
      if (value_node != nullptr) {
        Symbol *left_symbol = symbol_table->get_symbol(binary_operation_node->left->get_id());
        if (left_symbol) {
          symbol_table->add_symbol(new Symbol(binary_operation_node, scope_node->get_id(), SymbolTrait::BehavesLikeObject, value_node->identifier_value, left_symbol));
        } else {
          debug_out << "warning: missing symbol for value" << binary_operation_node->left->get_id() << " in dot operation " << binary_operation_node->get_id() << std::endl;
        }
      } else {
        debug_out << "warning: got unexpected dot operand" << binary_operation_node->right->get_id() << " in dot operation " << binary_operation_node->get_id() << std::endl;
      }
    } else if (binary_operation_node->binary_operator == Operator::Subscript) {
      // At this stage, even though the language only supports int arrays, assume that an array subscript
      // has the type of the symbol it subscripts
      Symbol *left_symbol = symbol_table->get_symbol(binary_operation_node->left->get_id());
      if (left_symbol) {
        int traits = (left_symbol->traits & SymbolTrait::IntLike) | (left_symbol->traits & SymbolTrait::BooleanLike) | (left_symbol->traits & SymbolTrait::StringLike);
        symbol_table->add_symbol(new Symbol(binary_operation_node, scope_node->get_id(), traits));
      } else {
        debug_out << "warning: missing symbol for value" << binary_operation_node->left->get_id() << " in subscript " << binary_operation_node->get_id() << std::endl;
      }
    } else {
      debug_out << "warning: unhandled binary operator for expression: " << binary_operation_node->binary_operator << std::endl;
    }

    return;
  }

  const auto &unary_operation_node = dynamic_cast<const UnaryOperationNode *>(expression_node);
  if (unary_operation_node != nullptr) {
    generate_symbols_for_expression(symbol_table, unary_operation_node->operand, scope_node);
    if (unary_operation_node->unary_operator == Operator::Negate)
      symbol_table->add_symbol(new Symbol(unary_operation_node, scope_node->get_id(), SymbolTrait::BooleanLike));
    else if (unary_operation_node->unary_operator == Operator::Negative)
      symbol_table->add_symbol(new Symbol(unary_operation_node, scope_node->get_id(), SymbolTrait::IntLike));
    else
      debug_out << "warning: unhandled unary operator for expression: " << unary_operation_node->unary_operator << std::endl;
    return;
  }

  const auto &array_initialization_node = dynamic_cast<const ArrayInitializationNode *>(expression_node);
  if (array_initialization_node != nullptr) {
    generate_symbols_for_expression(symbol_table, array_initialization_node->initialization_expression, scope_node);
    int traits = SymbolTrait::Subscriptable;
    if (array_initialization_node->type->type == "int")
      traits |= SymbolTrait::IntLike;
    if (array_initialization_node->type->type == "boolean")
      traits |= SymbolTrait::BooleanLike;
    symbol_table->add_symbol(new Symbol(array_initialization_node, scope_node->get_id(), traits));
    return;
  }

  const auto &class_initialization_node = dynamic_cast<const ClassInitializationNode *>(expression_node);
  if (class_initialization_node != nullptr) {
    symbol_table->add_symbol(new Symbol(class_initialization_node, scope_node->get_id(), SymbolTrait::Accessible));
    return;
  }

  const auto &method_call = dynamic_cast<const MethodCallNode *>(expression_node);
  if (method_call != nullptr) {
    symbol_table->add_symbol(new Symbol(method_call, scope_node->get_id(), SymbolTrait::BehavesLikeNode, method_call->value));
    return;
  }

  debug_out << "warning: unhandled expression generation " << expression_node->get_id() << std::endl;
}
