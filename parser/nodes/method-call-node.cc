#include <fstream>

#include "method-call-node.hpp"
using namespace mjavac::nodes;

MethodCallNode::MethodCallNode() {
  this->value = nullptr;
  this->is_new = false;
}

void MethodCallNode::generate_parse_graph(std::ofstream &stream) const {
  stream << this->get_id() << " [label=\"call";

  if (this->is_new)
    stream << " (new)";
  stream << "\"];" << std::endl;

  stream << this->get_id() << " -> " << this->value->get_id() << " [label=\"value\"];" << std::endl;
  this->value->generate_parse_graph(stream);

  for (const auto &parameter : this->parameters) {
    stream << this->get_id() << " -> " << parameter->get_id() << " [label=\"parameter\"];" << std::endl;
    parameter->generate_parse_graph(stream);
  }
}
