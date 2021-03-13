#include "ir-generator.hpp"

#include "program.hpp"

Program::Program(const ProgramNode *program_node) {
  for (const auto &class_node : program_node->declarations) {
    for (const auto &method_declaration_node : class_node->method_declarations) {
      ControlFlowGraph *cfg = new ControlFlowGraph();
      generate_method_ir(cfg, method_declaration_node);
      this->cfgs[class_node->identifier + "." + method_declaration_node->identifier] = cfg;
    }
  }
}

void Program::write(std::ostream &stream) const {
  stream << "digraph {" << std::endl;
  stream << "graph [splines=ortho]" << std::endl;

  for (const auto &[key, cfg] : this->cfgs) {
    stream << "subgraph \"cluster" << key << "\" {" << std::endl;
    stream << "label = \"" << key << "\";" << std::endl;
    cfg->write(stream);
    stream << "}" << std::endl;
  }

  stream << "}" << std::endl;
}
