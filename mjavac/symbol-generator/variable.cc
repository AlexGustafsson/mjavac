#include <iostream>

#include "../debug.hpp"

#include "../symbol-generator.hpp"

void generate_symbols_for_variable(SymbolTable *symbol_table, const VariableNode *variable_node, const Node *scope_node) {
  int traits = 0;
  if (variable_node->type->is_array)
    traits |= SymbolTrait::Subscriptable;
  if (variable_node->type->type == "int")
    traits |= SymbolTrait::IntLike;
  if (variable_node->type->type == "boolean")
    traits |= SymbolTrait::BooleanLike;
  // TODO: Handle class instances properly
  // if (variable_node->type->type == "identifier")
  //   traits |= SymbolTrait::BehavesLikeIdentifier;
  symbol_table->add_symbol(new Symbol(variable_node, variable_node->identifier, scope_node->get_id(), traits));
}
