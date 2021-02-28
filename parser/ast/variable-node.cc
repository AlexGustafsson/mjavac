#include "variable-node.hpp"
using namespace mjavac::ast;

VariableNode::VariableNode(TypeNode *type, std::string identifier) {
  this->type = type;
  this->identifier = identifier;
}

void VariableNode::generate_parse_graph(std::ostream &stream) const {
  stream << this->get_id() << " [label=\"declare " << this->identifier << "\"];" << std::endl;

  stream << this->get_id() << " -> " << this->type->get_id() << "[label=\"type\"];" << std::endl;
  this->type->generate_parse_graph(stream);
}
