#ifndef IR_GENERATOR_H
#define IR_GENERATOR_H

#include <mjavac/ast/nodes.hpp>

#include "cfg.hpp"
#include "symbol-table.hpp"

using namespace mjavac::ast;

void generate_method_ir(ControlFlowGraph *cfg, const MethodDeclarationNode *MethodDeclarationNode);

#endif
