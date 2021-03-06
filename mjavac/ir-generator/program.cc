#include <mjavac/ast/nodes.hpp>

#include "../ir-generator.hpp"
#include "mjavac/ast/conditional-node.hpp"

using namespace mjavac::ast;
using namespace mjavac::ir;

Address *generate_expression_ir(ControlFlowGraph *cfg, BasicBlock *current_block, const Node *expression_node) {
  return nullptr;
}

BasicBlock *generate_statement_ir(ControlFlowGraph *cfg, BasicBlock *current_block, const Node *statement_node) {
  const auto &binary_operation_node = dynamic_cast<const BinaryOperationNode *>(statement_node);
  if (binary_operation_node != nullptr) {
    if (binary_operation_node->binary_operator == Operator::Assign) {
      const auto &variable_node = dynamic_cast<const VariableNode *>(binary_operation_node->left);
      if (variable_node == nullptr)
        return current_block;

      Address *target = new Variable(variable_node->identifier);
      Address *result = generate_expression_ir(cfg, current_block, binary_operation_node->right);
      current_block->add_code(new Copy(target, result));
      current_block->set_identifier(variable_node->identifier, target);
    }
  }

  const auto &conditional_node = dynamic_cast<const ConditionalNode *>(statement_node);
  if (conditional_node != nullptr) {
    Address *result = generate_expression_ir(cfg, current_block, conditional_node->expression);
    current_block->add_code(new Expression(result, new Constant(1), "<"));

    BasicBlock *rejoinder = new BasicBlock();

    BasicBlock *positive_branch = new BasicBlock();
    positive_branch->positive_branch = rejoinder;
    current_block->positive_branch = positive_branch;
    // TODO: Evaluate statements

    if (conditional_node->next == nullptr) {
      current_block->negative_branch = rejoinder;
    } else {
      BasicBlock *negative_branch = new BasicBlock();
      current_block->negative_branch = negative_branch;
      // TODO: Evaluate statements
    }

    return rejoinder;
  }

  const auto &loop_node = dynamic_cast<const LoopNode *>(statement_node);
  if (loop_node != nullptr) {
    BasicBlock *header = new BasicBlock();
    Address *result = generate_expression_ir(cfg, header, conditional_node->expression);
    header->add_code(new Expression(result, new Constant(1), "<"));
    current_block->positive_branch = header;

    BasicBlock *body = new BasicBlock();
    header->positive_branch = body;

    // TODO: Evaluate statements

    BasicBlock *rejoinder = new BasicBlock();
    header->negative_branch = rejoinder;

    return rejoinder;
  }

  return current_block;
}

void generate_program_ir(ControlFlowGraph *cfg, const ProgramNode *program_node) {
  BasicBlock *current_block = cfg->entry_point;
  for (const auto &class_node : program_node->declarations) {
    for (const auto &method_declaration_node : class_node->method_declarations) {
      for (const auto &statement_node : method_declaration_node->statements)
        current_block = generate_statement_ir(cfg, current_block, statement_node);
    }
  }
}
