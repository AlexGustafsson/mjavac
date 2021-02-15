#include "variable-node.hpp"
using namespace mjavac::nodes;

VariableNode::VariableNode(std::string type, std::string identifier, bool is_declaration, bool is_array) {
  this->type = type;
  this->identifier = identifier;
  this->is_array = is_array;
  this->assigned_value = nullptr;
  this->is_declaration = is_declaration;
}

void VariableNode::generate_parse_graph(std::ofstream &stream) const {
  stream << this->get_id() << " [label=\"";
  if (this->is_declaration)
    stream << "declare ";
  else if (this->assigned_value != nullptr)
    stream << "set ";
  stream << this->type;
  if (this->is_array)
    stream << "[]";
  stream << " " << this->identifier << "\"];" << std::endl;

  if (this->assigned_value != nullptr) {
    stream << this->get_id() << " -> " << this->assigned_value->get_id() << " [label=\"assigned value\"];" << std::endl;
    this->assigned_value->generate_parse_graph(stream);
  }
}
