#ifndef CFG_H
#define CFG_H

#include <list>
#include <ostream>

#include "ir.hpp"

class BasicBlock {
public:
  std::list<mjavac::ir::ThreeAddressCode *> codes;
  BasicBlock *parent;
  BasicBlock *positive_branch;
  BasicBlock *negative_branch;

  BasicBlock();
  BasicBlock(BasicBlock *positive_branch);
  BasicBlock(BasicBlock *positive_branch, BasicBlock *negative_branch);

  void write(std::ostream &stream) const;

  intptr_t get_id() const;
};

class ControlFlowGraph {
private:
  BasicBlock *current_block;

public:
  BasicBlock *entry_point;

  ControlFlowGraph();

  BasicBlock *push_positive_branch();
  BasicBlock *push_negative_branch();
  BasicBlock *pop_branch();

  void add_code(mjavac::ir::ThreeAddressCode *code);

  void write(std::ostream &stream) const;

  intptr_t get_id() const;
};

#endif
