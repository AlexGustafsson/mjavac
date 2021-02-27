#ifndef SYMBOL_GENERATOR_H
#define SYMBOL_GENERATOR_H

#include <mjavac/ast/nodes.hpp>

#include "symbol-table.hpp"

using namespace mjavac::ast;

void generate_symbols_for_program(SymbolTable *symbol_table, const ProgramNode *program_node);

void generate_symbols_for_class(SymbolTable *symbol_table, const ClassDeclarationNode *class_node, const ProgramNode *program_node);

void generate_symbols_for_variable(SymbolTable *symbol_table, const VariableNode *variable_node, const Node *scope_node);

void generate_symbols_for_method(SymbolTable *symbol_table, const MethodDeclarationNode *method_node, const ClassDeclarationNode *class_node);

void generate_symbols_for_statement(SymbolTable *symbol_table, const Node *statement_node, const Node *method_node);

void generate_symbols_for_expression(SymbolTable *symbol_table, const Node *expression_node, const Node *scope_node);

#endif
