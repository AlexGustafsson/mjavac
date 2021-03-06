#include "cfg.hpp"

BasicBlock::BasicBlock()
    : positive_branch(nullptr), negative_branch(nullptr) {
}
BasicBlock::BasicBlock(BasicBlock *positive_branch)
    : positive_branch(positive_branch), negative_branch(nullptr) {
}
BasicBlock::BasicBlock(BasicBlock *positive_branch, BasicBlock *negative_branch)
    : positive_branch(positive_branch), negative_branch(negative_branch) {
}

intptr_t BasicBlock::get_id() const {
  return reinterpret_cast<intptr_t>(this);
}

void BasicBlock::add_code(mjavac::ir::ThreeAddressCode *code) {
  this->codes.push_back(code);
}

void BasicBlock::set_identifier(std::string identifier, Address *result) {
  this->identifiers[identifier] = result;
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
