#include "../ir-generator.hpp"

using namespace mjavac::ir;

void generate_program_ir(ControlFlowGraph *cfg, const SymbolTable *symbol_table, const ProgramNode *program_node) {
  cfg->push_positive_branch();
  cfg->add_code(new Push(new Constant(1)));

  cfg->pop_branch();
  cfg->push_negative_branch();
  cfg->push_positive_branch();
  cfg->add_code(new Expression(new Constant(1), new Constant(2), "+"));
  cfg->add_code(new Expression(new Variable("test"), new Constant(3), "-"));
  cfg->add_code(new Expression(new Constant(5), new TemporaryVariable(45678), "/"));
  cfg->add_code(new Copy(new Constant(7)));
}
