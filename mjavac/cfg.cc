#include "cfg.hpp"

BasicBlock::BasicBlock()
    : parent(nullptr), positive_branch(nullptr), negative_branch(nullptr) {
}
BasicBlock::BasicBlock(BasicBlock *positive_branch)
    : parent(nullptr), positive_branch(positive_branch), negative_branch(nullptr) {
}
BasicBlock::BasicBlock(BasicBlock *positive_branch, BasicBlock *negative_branch)
    : parent(nullptr), positive_branch(positive_branch), negative_branch(negative_branch) {
}

intptr_t BasicBlock::get_id() const {
  return reinterpret_cast<intptr_t>(this);
}

void BasicBlock::write(std::ostream &stream) const {
  stream << this->get_id() << "[shape=box label=\"";

  for (const auto &code : this->codes) {
    code->write(stream);
    stream << "\\n";
  }

  stream << "\"];" << std::endl;

  if (this->positive_branch != nullptr) {
    stream << this->get_id() << " -> " << this->positive_branch->get_id() << "[xlabel=\"true\"];" << std::endl;
    this->positive_branch->write(stream);
  }

  if (this->negative_branch != nullptr) {
    stream << this->get_id() << " -> " << this->negative_branch->get_id() << "[xlabel=\"false\"];" << std::endl;
    this->negative_branch->write(stream);
  }
}

ControlFlowGraph::ControlFlowGraph() {
  this->entry_point = new BasicBlock();
  this->current_block = this->entry_point;
}

BasicBlock *ControlFlowGraph::push_positive_branch() {
  BasicBlock *block = new BasicBlock();
  block->parent = this->current_block;
  this->current_block->positive_branch = block;
  this->current_block = block;
  return block;
}

BasicBlock *ControlFlowGraph::push_negative_branch() {
  BasicBlock *block = new BasicBlock();
  block->parent = this->current_block;
  this->current_block->negative_branch = block;
  this->current_block = block;
  return block;
}

BasicBlock *ControlFlowGraph::pop_branch() {
  if (this->current_block->parent != nullptr)
    this->current_block = this->current_block->parent;
  return this->current_block;
}

void ControlFlowGraph::add_code(mjavac::ir::ThreeAddressCode *code) {
  this->current_block->codes.push_back(code);
}

void ControlFlowGraph::write(std::ostream &stream) const {
  stream << "digraph {" << std::endl;

  stream << "graph [splines=ortho]" << std::endl;

  this->entry_point->write(stream);

  stream << "}" << std::endl;
}

intptr_t ControlFlowGraph::get_id() const {
  return reinterpret_cast<intptr_t>(this);
}
