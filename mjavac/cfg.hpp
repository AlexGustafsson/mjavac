#ifndef CFG_H
#define CFG_H

#include <list>
#include <map>
#include <ostream>

#include "ir.hpp"

using namespace mjavac::ir;

class BasicBlock {
public:
  std::map<std::string, Address *> identifiers;
  std::list<mjavac::ir::ThreeAddressCode *> codes;
  BasicBlock *positive_branch;
  BasicBlock *negative_branch;
  std::string identifier;

  BasicBlock();
  BasicBlock(std::string identifier);
  BasicBlock(BasicBlock *positive_branch);
  BasicBlock(BasicBlock *positive_branch, BasicBlock *negative_branch);

  void add_code(mjavac::ir::ThreeAddressCode *code);
  void set_identifier(std::string identifier, Address *result);

  void write(std::ostream &stream, std::map<long, bool> &visited) const;

  intptr_t get_id() const;
};

class ControlFlowGraph {
public:
  BasicBlock *entry_point;

  ControlFlowGraph();

  void write(std::ostream &stream) const;
  void write_standalone(std::ostream &stream) const;

  intptr_t get_id() const;
};

#endif
