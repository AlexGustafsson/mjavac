#include <fstream>

#include "conditional-node.hpp"
using namespace mjavac::nodes;

ConditionalNode::ConditionalNode(Node *expression, Node *positive_expression, Node *negative_expression) {
  this->expression = expression;
  this->positive_expression = positive_expression;
  this->negative_expression = negative_expression;
}

void ConditionalNode::generate_parse_graph(std::ofstream &stream) const {
  stream << this->get_id() << " [label=\"If case\"];" << std::endl;
  stream << this->get_id() << " -> " << this->expression << " [label=\"expression\"];" << std::endl;
  stream << this->get_id() << " -> " << this->positive_expression << " [label=\"positive_expression\"];" << std::endl;
  stream << this->get_id() << " -> " << this->negative_expression << " [label=\"negative_expression\"];" << std::endl;
  this->expression->generate_parse_graph(stream);
  this->positive_expression->generate_parse_graph(stream);
  this->negative_expression->generate_parse_graph(stream);
}
