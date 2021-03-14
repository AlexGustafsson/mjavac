#ifndef BYTECODE_GENERATOR_H
#define BYTECODE_GENERATOR_H

#include <map>

#include <mjavac/bytecode.hpp>

#include "cfg.hpp"

using namespace mjavac::vm;

void generate_bytecode(std::map<std::string, ControlFlowGraph *> &cfgs, ControlFlowGraph *entry_point, Bytecode *bytecode);

#endif
