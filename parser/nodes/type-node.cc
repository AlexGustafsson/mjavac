#include "type-node.hpp"
using namespace mjavac::nodes;

TypeNode::TypeNode(std::string type, bool is_array) {
  this->type = type;
  this->is_array = is_array;
}

void TypeNode::generate_parse_graph(std::ostream &stream) const {
  stream << this->get_id() << " [label=\"Type ";
  stream << type;
  if (this->is_array)
    stream << "[]";
  stream << "\"];" << std::endl;
}
