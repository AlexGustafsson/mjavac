#include <iostream>

#include "../debug.hpp"

#include "../symbol-generator.hpp"

void generate_symbols_for_statement(SymbolTable *symbol_table, const Node *statement_node, const Node *scope_node) {
  const auto &loop_node = dynamic_cast<const LoopNode *>(statement_node);
  if (loop_node != nullptr) {
    // Generate a symbol (and scope) for the condition
    symbol_table->add_symbol(new Symbol(loop_node, scope_node->get_id(), SymbolTrait::None));
    // Let the expression be part of the same scope as the loop itself
    generate_symbols_for_expression(symbol_table, loop_node->expression, loop_node);
    // Add all statements to the loop's scope
    for (const auto &loope_statement_node : loop_node->statements)
      generate_symbols_for_statement(symbol_table, loope_statement_node, loop_node);
    return;
  }

  const auto &conditional_node = dynamic_cast<const ConditionalNode *>(statement_node);
  if (conditional_node != nullptr) {
    // Generate a symbol (and scope) for the condition
    symbol_table->add_symbol(new Symbol(conditional_node, scope_node->get_id(), SymbolTrait::None));
    // Let the expression be part of the same scope as the condition itself
    generate_symbols_for_expression(symbol_table, conditional_node->expression, scope_node);
    // Add all statements to the condition's scope
    for (const auto &nested_statement_node : conditional_node->statements)
      generate_symbols_for_statement(symbol_table, nested_statement_node, conditional_node);
    // For now, only "else" is implemented, meaning there's no need to loop over chained if statements
    if (conditional_node->next != nullptr) {
      for (const auto &nested_statement_node : conditional_node->next->statements)
        generate_symbols_for_statement(symbol_table, nested_statement_node, conditional_node);
    }
    return;
  }

  const auto &variable_node = dynamic_cast<const VariableNode *>(statement_node);
  if (variable_node != nullptr) {
    generate_symbols_for_variable(symbol_table, variable_node, scope_node);
    return;
  }

  const auto &binary_operation_node = dynamic_cast<const BinaryOperationNode *>(statement_node);
  if (binary_operation_node != nullptr && binary_operation_node->binary_operator == Operator::Assign) {
    // If the assign is for a new declaration, handle it appropriately
    // otherwise, handle it like any other assignment
    const auto &variable_node = dynamic_cast<const VariableNode *>(binary_operation_node->left);
    if (variable_node == nullptr)
      generate_symbols_for_expression(symbol_table, binary_operation_node->left, scope_node);
    else
      generate_symbols_for_variable(symbol_table, variable_node, scope_node);
    generate_symbols_for_expression(symbol_table, binary_operation_node->right, scope_node);
    return;
  }

  const auto &return_node = dynamic_cast<const ReturnNode *>(statement_node);
  if (return_node != nullptr) {
    if (return_node->value != nullptr)
      generate_symbols_for_expression(symbol_table, return_node->value, scope_node);
    return;
  }

  generate_symbols_for_expression(symbol_table, statement_node, scope_node);
}
