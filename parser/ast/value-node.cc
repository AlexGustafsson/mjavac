#include <ostream>

#include "value-node.hpp"
using namespace mjavac::ast;

int ValueNode::Integer = 0;
int ValueNode::Boolean = 1;
int ValueNode::Identifier = 2;

ValueNode::ValueNode(int type, std::string value) {
  this->type = type;
  this->is_array = false;

  if (type == ValueNode::Integer)
    this->integer_value = std::stoi(value);
  else if (type == ValueNode::Boolean)
    this->boolean_value = value.compare("true") == 0;
  else if (type == ValueNode::Identifier)
    this->identifier_value = value;
}

void ValueNode::generate_parse_graph(std::ostream &stream) const {
  stream << this->get_id() << " [label=\"";

  if (this->type == ValueNode::Integer)
    stream << "int " << this->integer_value;
  else if (this->type == ValueNode::Boolean)
    stream << "bool " << this->boolean_value;
  else if (this->type == ValueNode::Identifier)
    stream << "identifier " << this->identifier_value;

  if (this->is_array)
    stream << "[]";

  stream << "\"];" << std::endl;
}
