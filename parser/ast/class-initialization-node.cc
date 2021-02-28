#include "class-initialization-node.hpp"
using namespace mjavac::ast;

ClassInitializationNode::ClassInitializationNode(std::string identifier) {
  this->identifier = identifier;
}

void ClassInitializationNode::generate_parse_graph(std::ostream &stream) const {
  stream << this->get_id() << " [label=\"new " << this->identifier << "\"];" << std::endl;
}
