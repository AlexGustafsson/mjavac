#include <mjavac/virtual-machine.hpp>

#include "bytecode-generator.hpp"
#include "ir-generator.hpp"

#include "program.hpp"

Program::Program(const ProgramNode *program_node) {
  this->program_node = program_node;
  for (const auto &class_node : program_node->declarations) {
    if (class_node->internal)
      continue;

    for (const auto &method_declaration_node : class_node->method_declarations) {
      ControlFlowGraph *cfg = new ControlFlowGraph();
      generate_method_ir(cfg, class_node, method_declaration_node);
      this->cfgs[class_node->identifier + "." + method_declaration_node->identifier] = cfg;
      if (method_declaration_node->identifier.compare("main") == 0)
        this->entry_point = cfg;
    }
  }

  this->bytecode = new mjavac::vm::Bytecode();
  generate_bytecode(this->cfgs, this->entry_point, this->bytecode);
}

void Program::execute() const {
  VirtualMachine *vm = new VirtualMachine(this->bytecode);
  vm->execute();
}

void Program::write_bytecode(std::ostream &stream) const {
  this->bytecode->write(stream);
}

void Program::write_cfg(std::ostream &stream) const {
  stream << "digraph {" << std::endl;
  stream << "graph [splines=ortho]" << std::endl;
  stream << "compound=true;" << std::endl;

  for (const auto &[key, cfg] : this->cfgs) {
    if (cfg == this->entry_point) {
      stream << "entrypoint[shape=box label=\"Entry Point\"];" << std::endl;
      stream << "entrypoint -> " << cfg->entry_point->get_id() << ";" << std::endl;
    }
    stream << "subgraph \"cluster" << key << "\" {" << std::endl;
    stream << "label = \"" << key << "\";" << std::endl;
    cfg->write(stream);
    stream << "}" << std::endl;
  }

  stream << "}" << std::endl;
}
