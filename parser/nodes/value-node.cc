#include <fstream>

#include "value-node.hpp"
using namespace mjavac::nodes;

int ValueNode::Integer = 0;
int ValueNode::Boolean = 1;
int ValueNode::Identifier = 2;
int ValueNode::Object = 3;

ValueNode::ValueNode(int type, std::string value) {
  this->type = type;

  if (type == ValueNode::Integer)
    this->integer_value = std::stoi(value);
  else if (type == ValueNode::Boolean)
    this->boolean_value = value.compare("true") == 0;
  else if (type == ValueNode::Identifier)
    this->identifier_value = value;
}

ValueNode::ValueNode(int type, std::list<std::string> value) {
  this->type = type;

  if (type == ValueNode::Object)
    this->object_value = value;
}

void ValueNode::generate_parse_graph(std::ofstream &stream) const {
  stream << this->get_id() << " [label=\"";

  if (this->type == ValueNode::Integer)
    stream << "int " << this->integer_value;
  else if (this->type == ValueNode::Boolean)
    stream << "bool " << this->boolean_value;
  else if (this->type == ValueNode::Identifier)
    stream << "identifier " << this->identifier_value;
  else if (this->type == ValueNode::Object) {
    for (auto iterator = this->object_value.begin(); iterator != this->object_value.end(); ++iterator) {
      stream << iterator->c_str();
      if (next(iterator) != this->object_value.end())
        stream << '.';
    }
  }

  if (this->is_array)
    stream << "[]";

  stream << "\"];" << std::endl;

  if (this->is_array) {
    stream << this->get_id() << " -> " << this->array_index->get_id() << " [label=\"index\"];" << std::endl;
    this->array_index->generate_parse_graph(stream);
  }
}
