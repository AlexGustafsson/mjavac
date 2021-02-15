#include "symbol-generator.hpp"

void generate_symbols_for_program(SymbolTable *symbol_table, const ProgramNode *program_node) {
  // Add the program's symbol
  symbol_table->symbols[program_node->get_id()] = new Symbol(program_node, program_node->get_id(), SymbolTrait::None);

  // Add each class
  for (const auto &class_node : program_node->declarations)
    generate_symbols_for_class(symbol_table, class_node, program_node);
}

void generate_symbols_for_class(SymbolTable *symbol_table, const ClassDeclarationNode *class_node, const ProgramNode *program_node) {
  // Add a symbol for the class
  symbol_table->symbols[class_node->get_id()] = new Symbol(class_node, program_node->get_id(), SymbolTrait::Accessible | SymbolTrait::Initializable);

  // Add symbols for variables
  for (const auto &variable_node : class_node->variable_declarations)
    generate_symbols_for_variable(symbol_table, variable_node, class_node);

  // Add symbols for methods
  for (const auto &method_node : class_node->method_declarations)
    generate_symbols_for_method(symbol_table, method_node, class_node);
}

void generate_symbols_for_variable(SymbolTable *symbol_table, const VariableDeclarationNode *variable_node, const Node *scope_node) {
  int traits = SymbolTrait::None;
  if (variable_node->is_array)
    traits |= SymbolTrait::Accessible;
  if (variable_node->type == "int")
    traits |= SymbolTrait::IntLike;
  symbol_table->symbols[variable_node->get_id()] = new Symbol(variable_node, variable_node->identifier, scope_node->get_id(), traits);
}

void
generate_symbols_for_method(SymbolTable *symbol_table, const MethodDeclarationNode *method_node, const ClassDeclarationNode *class_node) {
  // Add a symbol for the method itself
  symbol_table->symbols[method_node->get_id()] = new Symbol(method_node, method_node->identifier, class_node->get_id(), SymbolTrait::Callable);

  // Add parameters
  for (const auto &parameter_node : method_node->parameters)
    generate_symbols_for_variable(symbol_table, parameter_node, method_node);

  // Add statements
  for (const auto &statement_node : method_node->statements)
    generate_symbols_for_statement(symbol_table, statement_node, method_node);
}

void generate_symbols_for_statement(SymbolTable *symbol_table, const Node *statement_node, const Node *scope_node) {
  // const auto &loop_node = static_cast<mjavac::nodes::LoopNode *>(statement_node);
  // if (loop_node != nullptr) {
  //   // Add statements
  //   // ...
  //   continue;
  // }

  // const auto &conditional_node = static_cast<const ConditionalNode *>(statement_node);
  // if (conditional_node != nullptr) {
  //   for (const auto& nested_statement_node : conditional_node->statements)
  //     generate_symbols_for_statement(symbol_table, nested_statement_node, conditional_node);
  //   return;
  // }

  const auto &variable_node = dynamic_cast<const VariableDeclarationNode *>(statement_node);
  if (variable_node != nullptr) {
    generate_symbols_for_variable(symbol_table, variable_node, scope_node);
    return;
  }
}
