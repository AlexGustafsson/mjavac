#include "method-declaration-node.hpp"
using namespace nodes;

MethodDeclarationNode::MethodDeclarationNode(std::string type, std::string identifier) {
  this->type = type;
  this->identifier = identifier;
}

void MethodDeclarationNode::generate_parse_graph(std::ofstream &stream) const {
  stream << this->get_id() << " [label=\"" << this->type << " " << this->identifier << "()\"];" << std::endl;

  for (auto node : this->parameters) {
    stream << this->get_id() << " -> " << node->get_id() << "[label=\"parameter\"];" << std::endl;
    node->generate_parse_graph(stream);
  }

  for (auto node : this->statements) {
    stream << this->get_id() << " -> " << node->get_id() << "[label=\"statement\"];" << std::endl;
    node->generate_parse_graph(stream);
  }
}
