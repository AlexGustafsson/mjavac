#include "array-initialization-node.hpp"
using namespace mjavac::ast;

ArrayInitializationNode::ArrayInitializationNode(TypeNode *type, Node *initialization_expression) {
  this->type = type;
  this->initialization_expression = initialization_expression;
}

void ArrayInitializationNode::generate_parse_graph(std::ostream &stream) const {
  stream << this->get_id() << " [label=\"new array\"];" << std::endl;

  stream << this->get_id() << " -> " << this->type->get_id() << "[label=\"type\"];" << std::endl;
  this->type->generate_parse_graph(stream);

  stream << this->get_id() << " -> " << this->initialization_expression->get_id() << "[label=\"initialization expression\"];" << std::endl;
  this->initialization_expression->generate_parse_graph(stream);
}
