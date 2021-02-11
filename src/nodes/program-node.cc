#include "program-node.hpp"
using namespace nodes;

ProgramNode::ProgramNode() {

}

void ProgramNode::generate_parse_graph(std::ofstream &stream) const {
  for (auto class_node : this->declarations) {
    stream << this->get_id() << " -> " << class_node->get_id() << ";" << std::endl;
    class_node->generate_parse_graph(stream);
  }
}
