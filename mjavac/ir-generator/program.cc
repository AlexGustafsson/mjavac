#include <mjavac/ast/nodes.hpp>
#include <sstream>

#include "../ir-generator.hpp"
#include "mjavac/ast/conditional-node.hpp"
#include "mjavac/ast/value-node.hpp"
#include "mjavac/ast/variable-node.hpp"

using namespace mjavac::ast;
using namespace mjavac::ir;

Address *generate_expression_ir(ControlFlowGraph *cfg, BasicBlock *current_block, const Node *expression_node) {
  const auto &binary_operation_node = dynamic_cast<const BinaryOperationNode *>(expression_node);
  if (binary_operation_node != nullptr) {
    if (binary_operation_node->binary_operator == Operator::Subscript) {
      // Not supported
    } else {
      Address *left = generate_expression_ir(cfg, current_block, binary_operation_node->left);
      Address *right = generate_expression_ir(cfg, current_block, binary_operation_node->right);
      Address *target = new TemporaryVariable(binary_operation_node->get_id());
      std::stringstream stream;
      stream << binary_operation_node->binary_operator;
      current_block->add_code(new Expression(target, left, right, stream.str()));
      return target;
    }
  }

  const auto &unary_operation_node = dynamic_cast<const UnaryOperationNode *>(expression_node);
  if (unary_operation_node != nullptr) {
    if (unary_operation_node->unary_operator == Operator::Negate) {
      Address *operand = generate_expression_ir(cfg, current_block, unary_operation_node->operand);
      Address *target = new TemporaryVariable(unary_operation_node->get_id());
      current_block->add_code(new UnaryExpression(target, operand, "!"));
      return target;
    } else if (unary_operation_node->unary_operator == Operator::Negative) {
      Address *left = new Constant(0);
      Address *right = generate_expression_ir(cfg, current_block, unary_operation_node->operand);
      Address *target = new TemporaryVariable(unary_operation_node->get_id());
      current_block->add_code(new Expression(target, left, right, "-"));
      return target;
    }
  }

  const auto &value_node = dynamic_cast<const ValueNode *>(expression_node);
  if (value_node != nullptr) {
    if (value_node->type == ValueNode::Integer) {
      return new Constant(value_node->integer_value);
    } else if (value_node->type == ValueNode::Boolean) {
      return new Constant(value_node->boolean_value ? 1 : 0);
    } else if (value_node->type == ValueNode::Identifier) {
      return new Variable(value_node->identifier_value);
    }
  }

  const auto &method_call_node = dynamic_cast<const MethodCallNode *>(expression_node);
  if (method_call_node != nullptr) {
    // Evaluate all parameters and push them to the stack
    for (const auto &parameter : method_call_node->parameters) {
      Address *result = generate_expression_ir(cfg, current_block, parameter);
      current_block->add_code(new Push(result));
    }
    // TODO: Add actual method name or id to the call
    current_block->add_code(new MethodCall(nullptr, new Constant(method_call_node->parameters.size())));
  }

  return nullptr;
}

BasicBlock *generate_statement_ir(ControlFlowGraph *cfg, BasicBlock *current_block, const Node *statement_node) {
  const auto &binary_operation_node = dynamic_cast<const BinaryOperationNode *>(statement_node);
  if (binary_operation_node != nullptr) {
    if (binary_operation_node->binary_operator == Operator::Assign) {
      // Variable definition
      const auto &variable_node = dynamic_cast<const VariableNode *>(binary_operation_node->left);
      if (variable_node != nullptr) {
        Address *target = new Variable(variable_node->identifier);
        Address *result = generate_expression_ir(cfg, current_block, binary_operation_node->right);
        current_block->add_code(new Copy(target, result));
        current_block->set_identifier(variable_node->identifier, target);
        return current_block;
      }

      // Assign to variable
      const auto &value_node = dynamic_cast<const ValueNode *>(binary_operation_node->left);
      if (value_node != nullptr && value_node->type == ValueNode::Identifier) {
        Address *target = new Variable(value_node->identifier_value);
        Address *result = generate_expression_ir(cfg, current_block, binary_operation_node->right);
        current_block->add_code(new Copy(target, result));
        current_block->set_identifier(value_node->identifier_value, target);
        return current_block;
      }
    }
  }

  const auto &conditional_node = dynamic_cast<const ConditionalNode *>(statement_node);
  if (conditional_node != nullptr) {
    Address *result = generate_expression_ir(cfg, current_block, conditional_node->expression);
    current_block->add_code(new Expression(new TemporaryVariable(conditional_node->get_id()), result, new Constant(1), "<"));

    BasicBlock *rejoinder = new BasicBlock();

    BasicBlock *positive_branch = new BasicBlock();
    current_block->positive_branch = positive_branch;
    // Evaluate statements, making sure the last branch ends up in the rejoinder
    BasicBlock *positive_branch_end = positive_branch;
    for (const auto &statement : conditional_node->statements)
      positive_branch_end = generate_statement_ir(cfg, positive_branch, statement);
    positive_branch_end->positive_branch = rejoinder;

    if (conditional_node->next == nullptr) {
      current_block->negative_branch = rejoinder;
    } else {
      BasicBlock *negative_branch = new BasicBlock();
      current_block->negative_branch = negative_branch;
      // Evaluate statements, making sure the last branch ends up in the rejoinder
      BasicBlock *negative_branch_end = negative_branch;
      for (const auto &statement : conditional_node->statements)
        negative_branch_end = generate_statement_ir(cfg, negative_branch, statement);
      negative_branch_end->positive_branch = rejoinder;
    }

    return rejoinder;
  }

  const auto &loop_node = dynamic_cast<const LoopNode *>(statement_node);
  if (loop_node != nullptr) {
    BasicBlock *header = new BasicBlock();
    Address *result = generate_expression_ir(cfg, header, loop_node->expression);
    // Compare the expression with < 1
    header->add_code(new Expression(new TemporaryVariable(loop_node->get_id()), result, new Constant(1), "<"));
    current_block->positive_branch = header;

    BasicBlock *body = new BasicBlock();
    header->positive_branch = body;

    // Evaluate statements, making sure the last branch ends up in the header
    BasicBlock *body_end = body;
    for (const auto &statement : loop_node->statements)
      body_end = generate_statement_ir(cfg, body, statement);
    body_end->positive_branch = header;

    BasicBlock *rejoinder = new BasicBlock();
    header->negative_branch = rejoinder;

    return rejoinder;
  }

  generate_expression_ir(cfg, current_block, statement_node);

  return current_block;
}

void generate_method_ir(ControlFlowGraph *cfg, const MethodDeclarationNode *method_declaration_node) {
  BasicBlock *current_block = cfg->entry_point;
  for (const auto &statement_node : method_declaration_node->statements)
    current_block = generate_statement_ir(cfg, current_block, statement_node);
}
