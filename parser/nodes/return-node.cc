#include "return-node.hpp"
using namespace mjavac::nodes;

ReturnNode::ReturnNode(Node *value) {
  this->value = value;
}

ReturnNode::ReturnNode() {
  this->value = nullptr;
}

void ReturnNode::generate_parse_graph(std::ostream &stream) const {
  stream << this->get_id() << " [label=\"return\"];" << std::endl;
  if (this->value != nullptr) {
    stream << this->get_id() << " -> " << this->value->get_id() << " [label=\"value\"];" << std::endl;
    this->value->generate_parse_graph(stream);
  }
}
