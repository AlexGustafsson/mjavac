#include "unary-operation-node.hpp"
using namespace mjavac::ast;

UnaryOperationNode::UnaryOperationNode(Node *operand, Operator unary_operator) {
  this->operand = operand;
  this->unary_operator = unary_operator;
}

void UnaryOperationNode::generate_parse_graph(std::ostream &stream) const {
  stream << this->get_id() << " [label=\"" << this->unary_operator << "\"];" << std::endl;
  stream << this->get_id() << " -> " << this->operand->get_id() << " [label=\"operand\"];" << std::endl;
  this->operand->generate_parse_graph(stream);
}
