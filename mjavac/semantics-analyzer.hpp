#ifndef SEMANTICS_ANALYZER_H
#define SEMANTICS_ANALYZER_H

#include <mjavac/parser.hpp>

#include "symbol-table.hpp"

using namespace mjavac::nodes;

bool analyze_program_semantics(const SymbolTable *symbol_table, const ProgramNode *program_node);
bool analyze_class_semantics(const SymbolTableView *view, const ProgramNode *program_node, const ClassDeclarationNode *class_node);

#endif
