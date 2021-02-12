#include "method-declaration-node.hpp"
using namespace mjavac::nodes;

MethodDeclarationNode::MethodDeclarationNode(bool is_public, bool is_static) {
  this->is_public = is_public;
  this->is_static = is_static;
}

void MethodDeclarationNode::generate_parse_graph(std::ofstream &stream) const {
  stream << this->get_id() << " [label=\"";

  if (this->is_public)
    stream << "public ";
  if (this->is_static)
    stream << "static ";

  stream << this->type << " " << this->identifier << "()\"];" << std::endl;

  for (auto node : this->parameters) {
    stream << this->get_id() << " -> " << node->get_id() << "[label=\"parameter\"];" << std::endl;
    node->generate_parse_graph(stream);
  }

  for (auto node : this->statements) {
    stream << this->get_id() << " -> " << node->get_id() << "[label=\"statement\"];" << std::endl;
    node->generate_parse_graph(stream);
  }
}
