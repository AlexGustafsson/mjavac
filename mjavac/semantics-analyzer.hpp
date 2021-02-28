#ifndef SEMANTICS_ANALYZER_H
#define SEMANTICS_ANALYZER_H

#include <mjavac/parser.hpp>

#include "symbol-table.hpp"

using namespace mjavac::ast;

bool analyze_program_semantics(const SymbolTable *symbol_table, const ProgramNode *program_node);
bool analyze_class_semantics(SymbolTableView *view, const ProgramNode *program_node, const ClassDeclarationNode *class_node);
bool analyze_method_semantics(SymbolTableView *view, const ProgramNode *program_node, const MethodDeclarationNode *method_node);
bool analyze_statement_semantics(SymbolTableView *view, const ProgramNode *program_node, const MethodDeclarationNode *method_node, const Node *statement);
bool analyze_expression_semantics(SymbolTableView *view, const ProgramNode *program_node, const Node *expression_node);

#endif
