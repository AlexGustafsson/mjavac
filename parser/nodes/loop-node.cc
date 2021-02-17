#include "loop-node.hpp"
using namespace mjavac::nodes;

LoopNode::LoopNode(Node *expression) {
  this->expression = expression;
}

void LoopNode::generate_parse_graph(std::ostream &stream) const {
  stream << this->get_id() << " [label=\"Loop\"];" << std::endl;
  stream << this->get_id() << " -> " << this->expression->get_id() << " [label=\"expression\"];" << std::endl;
  this->expression->generate_parse_graph(stream);

  for (const auto &statement : this->statements) {
    stream << this->get_id() << " -> " << statement->get_id() << " [label=\"statement\"];" << std::endl;
    statement->generate_parse_graph(stream);
  }
}
