#include "program-node.hpp"
using namespace mjavac::nodes;

ProgramNode::ProgramNode(Source *source) {
  this->source = source;
}

void ProgramNode::generate_parse_graph(std::ostream &stream) const {
  stream << "digraph {" << std::endl;
  stream << this->get_id() << "[label=\"Program\"];" << std::endl;
  for (const auto &class_node : this->declarations) {
    stream << this->get_id() << " -> " << class_node->get_id() << ";" << std::endl;
    class_node->generate_parse_graph(stream);
  }
  stream << "}" << std::endl;
}
