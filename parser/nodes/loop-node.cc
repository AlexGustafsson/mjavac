#include "loop-node.hpp"
using namespace mjavac::nodes;

LoopNode::LoopNode(Node *expression, Node *statement) {
  this->expression = expression;
  this->statement = statement;
}

void LoopNode::generate_parse_graph(std::ofstream &stream) const {
  stream << this->get_id() << " [label=\"Loop\"];" << std::endl;
  stream << this->get_id() << " -> " << this->expression << " [label=\"expression\"];" << std::endl;
  stream << this->get_id() << " -> " << this->statement << " [label=\"statement\"];" << std::endl;
  this->expression->generate_parse_graph(stream);
  this->statement->generate_parse_graph(stream);
}
