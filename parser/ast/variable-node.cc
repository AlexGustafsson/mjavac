#include "variable-node.hpp"
using namespace mjavac::ast;

VariableNode::VariableNode(TypeNode *type, std::string identifier, bool is_declaration) {
  this->type = type;
  this->identifier = identifier;
  this->assigned_value = nullptr;
  this->is_declaration = is_declaration;
}

void VariableNode::generate_parse_graph(std::ostream &stream) const {
  stream << this->get_id() << " [label=\"";
  if (this->is_declaration)
    stream << "declare ";
  else if (this->assigned_value != nullptr)
    stream << "set ";
  stream << this->identifier << "\"];" << std::endl;

  stream << this->get_id() << " -> " << this->type->get_id() << "[label=\"type\"];" << std::endl;
  this->type->generate_parse_graph(stream);

  if (this->assigned_value != nullptr) {
    stream << this->get_id() << " -> " << this->assigned_value->get_id() << " [label=\"assigned value\"];" << std::endl;
    this->assigned_value->generate_parse_graph(stream);
  }
}
