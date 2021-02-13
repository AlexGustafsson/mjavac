#include "variable-declaration-node.hpp"
using namespace mjavac::nodes;

VariableDeclarationNode::VariableDeclarationNode(std::string type, std::string identifier, bool is_array) {
  this->type = type;
  this->identifier = identifier;
  this->is_array = is_array;
  this->value = nullptr;
}

void VariableDeclarationNode::generate_parse_graph(std::ofstream &stream) const {
  stream << this->get_id() << " [label=\"" << this->type;
  if (this->is_array)
    stream << "[]";
  stream << " " << this->identifier << "\"];" << std::endl;

  if (this->value != nullptr) {
    stream << this->get_id() << " -> " << this->value->get_id() << " [label=\"value\"];" << std::endl;
    this->value->generate_parse_graph(stream);
  }
}
