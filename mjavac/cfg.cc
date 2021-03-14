#include "cfg.hpp"

BasicBlock::BasicBlock(std::string identifier)
  : positive_branch(nullptr), negative_branch(nullptr), identifier(identifier) {
}

BasicBlock::BasicBlock()
    : positive_branch(nullptr), negative_branch(nullptr) {
  this->identifier = std::to_string(this->get_id());
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

void BasicBlock::write(std::ostream &stream, std::map<long, bool> &visited) const {
  // Recursion guard
  if (visited.count(this->get_id()) > 0)
    return;
  visited[this->get_id()] = true;

  stream << this->get_id() << "[shape=box xlabel=\"" << this->identifier << "\", label=\"";
  for (const auto &code : this->codes) {
    code->write(stream);
    stream << "\\n";
  }

  stream << "\"];" << std::endl;

  if (this->positive_branch != nullptr) {
    stream << this->get_id() << " -> " << this->positive_branch->get_id() << "[xlabel=\"true\"];" << std::endl;
    this->positive_branch->write(stream, visited);
  }

  if (this->negative_branch != nullptr) {
    stream << this->get_id() << " -> " << this->negative_branch->get_id() << "[xlabel=\"false\"];" << std::endl;
    this->negative_branch->write(stream, visited);
  }
}

ControlFlowGraph::ControlFlowGraph() {
  this->entry_point = new BasicBlock();
}

void ControlFlowGraph::write_standalone(std::ostream &stream) const {
  stream << "digraph {" << std::endl;

  stream << "graph [splines=ortho]" << std::endl;

  this->write(stream);

  stream << "}" << std::endl;
}

void ControlFlowGraph::write(std::ostream &stream) const {
  std::map<long, bool> visited;
  this->entry_point->write(stream, visited);
}

intptr_t ControlFlowGraph::get_id() const {
  return reinterpret_cast<intptr_t>(this);
}
