#include "loop-node.hpp"
using namespace mjavac::nodes;

LoopNode::LoopNode(Node *expression, Node *loop_expression) {
  this->expression = expression;
  this->loop_expression = loop_expression;
}

void LoopNode::generate_parse_graph(std::ofstream &stream) const {
  stream << this->get_id() << " [label=\"Loop\"];" << std::endl;
  stream << this->get_id() << " -> " << this->expression << " [label=\"expression\"];" << std::endl;
  stream << this->get_id() << " -> " << this->loop_expression << " [label=\"loop_expression\"];" << std::endl;
  this->expression->generate_parse_graph(stream);
  this->loop_expression->generate_parse_graph(stream);
}
