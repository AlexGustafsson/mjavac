#include <ostream>

#include "method-call-node.hpp"
using namespace mjavac::ast;

MethodCallNode::MethodCallNode(Node *value) {
  this->value = value;
}

void MethodCallNode::generate_parse_graph(std::ostream &stream) const {
  stream << this->get_id() << " [label=\"call\"];" << std::endl;

  stream << this->get_id() << " -> " << this->value->get_id() << " [label=\"value\"];" << std::endl;
  this->value->generate_parse_graph(stream);

  for (const auto &parameter : this->parameters) {
    stream << this->get_id() << " -> " << parameter->get_id() << " [label=\"parameter\"];" << std::endl;
    parameter->generate_parse_graph(stream);
  }
}
