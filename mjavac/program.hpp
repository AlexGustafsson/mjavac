#ifndef PROGRAM_H
#define PROGRAM_H

#include "cfg.hpp"
#include <mjavac/ast/nodes.hpp>
#include <mjavac/bytecode.hpp>

using namespace mjavac::ast;
using namespace mjavac::vm;

class Program {
private:
  const ProgramNode *program_node;
  std::map<std::string, ControlFlowGraph *> cfgs;
  ControlFlowGraph *entry_point;
  Bytecode *bytecode;

public:
  Program(const ProgramNode *program_node);
  void write_cfg(std::ostream &stream) const;
  void write_bytecode(std::ostream &stream) const;
  void execute() const;
};

#endif
