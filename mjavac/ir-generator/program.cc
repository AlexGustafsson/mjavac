#include "../ir-generator.hpp"

using namespace mjavac::ir;

void generate_program_ir(ControlFlowGraph *cfg, const SymbolTable *symbol_table, const ProgramNode *program_node) {
  cfg->push_positive_branch();
  cfg->add_code(new Push(new Address()));

  cfg->pop_branch();
  cfg->push_negative_branch();
  cfg->push_positive_branch();
  cfg->add_code(new Expression(new Address(), new Address(), "+"));
  cfg->add_code(new Expression(new Address(), new Address(), "-"));
  cfg->add_code(new Expression(new Address(), new Address(), "/"));
  cfg->add_code(new Copy(new Address()));
}
