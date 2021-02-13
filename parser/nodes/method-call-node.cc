#include <fstream>

#include "method-call-node.hpp"
using namespace mjavac::nodes;

MethodCallNode::MethodCallNode() {}

void MethodCallNode::generate_parse_graph(std::ofstream &stream) const {
  stream << this->get_id() << " [label=\"";

  for (auto iterator = this->objects.begin(); iterator != this->objects.end(); ++iterator) {;
    stream << iterator->c_str();
    if (next(iterator) != this->objects.end())
      stream << '.';
  }
  stream << "()\"];" << std::endl;

  for (const auto& parameter : this->parameters) {
    stream << this->get_id() << " -> " << parameter->get_id() << " [label=\"parameter\"];" << std::endl;
    parameter->generate_parse_graph(stream);
  }
}
