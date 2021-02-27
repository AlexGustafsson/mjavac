#include "binary-operation-node.hpp"
using namespace mjavac::ast;

BinaryOperationNode::BinaryOperationNode(Node *left, Node *right, Operator binary_operator) {
  this->left = left;
  this->right = right;
  this->binary_operator = binary_operator;
}

void BinaryOperationNode::generate_parse_graph(std::ostream &stream) const {
  stream << this->get_id() << " [label=\"" << this->binary_operator << "\"];" << std::endl;
  stream << this->get_id() << " -> " << this->left->get_id() << " [label=\"left\"];" << std::endl;
  stream << this->get_id() << " -> " << this->right->get_id() << " [label=\"right\"];" << std::endl;
  this->left->generate_parse_graph(stream);
  this->right->generate_parse_graph(stream);
}
