#ifndef PROGRAM_H
#define PROGRAM_H

#include "cfg.hpp"
#include <mjavac/ast/nodes.hpp>

using namespace mjavac::ast;

class Program {
private:
  const ProgramNode *program_node;
  std::map<std::string, ControlFlowGraph*> cfgs;
public:
  Program(const ProgramNode *program_node);
  void write(std::ostream &stream) const;
};

#endif
