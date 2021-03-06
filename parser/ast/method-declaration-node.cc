#include "method-declaration-node.hpp"
using namespace mjavac::ast;

MethodDeclarationNode::MethodDeclarationNode(bool is_public, bool is_static) {
  this->is_public = is_public;
  this->is_static = is_static;
}

void MethodDeclarationNode::generate_parse_graph(std::ostream &stream) const {
  stream << this->get_id() << " [label=\"";

  if (this->is_public)
    stream << "public ";
  if (this->is_static)
    stream << "static ";
  stream << this->identifier << "()\"];" << std::endl;

  stream << this->get_id() << " -> " << this->type->get_id() << "[label=\"type\"];" << std::endl;
  this->type->generate_parse_graph(stream);

  for (const auto &node : this->parameters) {
    stream << this->get_id() << " -> " << node->get_id() << "[label=\"parameter\"];" << std::endl;
    node->generate_parse_graph(stream);
  }

  for (const auto &node : this->statements) {
    stream << this->get_id() << " -> " << node->get_id() << "[label=\"statement\"];" << std::endl;
    node->generate_parse_graph(stream);
  }
}
