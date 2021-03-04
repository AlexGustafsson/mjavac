#include <iostream>

#include "../debug.hpp"

#include "../symbol-generator.hpp"

void generate_symbols_for_class(SymbolTable *symbol_table, const ClassDeclarationNode *class_node, const ProgramNode *program_node) {
  // Add a symbol for the class
  symbol_table->add_symbol(new Symbol(class_node, class_node->identifier, program_node->get_id(), SymbolTrait::Accessible | SymbolTrait::Initializable));

  // Add symbols for variables
  for (const auto &variable_node : class_node->variable_declarations)
    generate_symbols_for_variable(symbol_table, variable_node, class_node);

  // Add symbols for variable assignments
  for (const auto &variable_assignment : class_node->variable_assignments) {
    const VariableNode *variable_node = dynamic_cast<const VariableNode *>(variable_assignment);
    if (variable_node == nullptr)
      continue; // TODO: Print error
    generate_symbols_for_variable(symbol_table, variable_node, class_node);
  }

  // Add symbols for methods
  for (const auto &method_node : class_node->method_declarations) {
    // Add a symbol for the method itself
    symbol_table->add_symbol(new Symbol(method_node, method_node->identifier, class_node->get_id(), SymbolTrait::Callable));
  }

  // Recurse after defining the "prototypes" to make them known in the scope
  for (const auto &method_node : class_node->method_declarations) {
    generate_symbols_for_method(symbol_table, method_node, class_node);
  }
}
