#include <string>
#include <map>

#include <mjavac/instructions.hpp>

#include "../bytecode-generator.hpp"

void push_address(Address *address, Block *block) {
  const Constant *constant = dynamic_cast<const Constant *>(address);
  if (constant != nullptr) {
    block->instructions.push_back(new Instruction_iconst(constant->value));
  }

  const TemporaryVariable *temporary_variable = dynamic_cast<const TemporaryVariable *>(address);
  if (temporary_variable != nullptr) {
    block->instructions.push_back(new Instruction_iload("_t" + std::to_string(temporary_variable->id)));
  }

  const Variable *variable = dynamic_cast<const Variable *>(address);
  if (variable != nullptr) {
    block->instructions.push_back(new Instruction_iload(variable->identifier));
  }
}

std::string resolve_variable(Address *address) {
  const TemporaryVariable *temporary_variable = dynamic_cast<const TemporaryVariable *>(address);
  if (temporary_variable != nullptr) {
    return "_t" + std::to_string(temporary_variable->id);
  }

  const Variable *variable = dynamic_cast<const Variable *>(address);
  if (variable != nullptr) {
    return variable->identifier;
  }

  // Never occurs as all possible paths are handled above
  return "";
}

void generate_block_bytecode(BasicBlock *block, Bytecode *bytecode, std::map<long, bool> &visited, bool is_entry_point) {
  if (visited.count(block->get_id()) > 0)
    return;
  visited[block->get_id()] = true;

  Block *bytecode_block = new Block(block->identifier);
  if (is_entry_point)
    bytecode->entry_point = bytecode_block;
  bytecode->blocks[block->identifier] = bytecode_block;
  for (const auto &code : block->codes) {
    const Expression *expression = dynamic_cast<const Expression *>(code);
    if (expression != nullptr) {
      // Resolve and push the operands
      push_address(expression->left, bytecode_block);
      push_address(expression->right, bytecode_block);
      // Calculate and push the result
      if (expression->ir_operator.compare("+") == 0)
        bytecode_block->instructions.push_back(new Instruction_iadd());
      else if (expression->ir_operator.compare("-") == 0)
        bytecode_block->instructions.push_back(new Instruction_isub());
      else if (expression->ir_operator.compare("*") == 0)
        bytecode_block->instructions.push_back(new Instruction_imul());
      else if (expression->ir_operator.compare("/") == 0)
        bytecode_block->instructions.push_back(new Instruction_idiv());
      else if (expression->ir_operator.compare("<") == 0)
        bytecode_block->instructions.push_back(new Instruction_ilt());
      else if (expression->ir_operator.compare("&&") == 0)
        bytecode_block->instructions.push_back(new Instruction_iand());
      else if (expression->ir_operator.compare("||") == 0)
        bytecode_block->instructions.push_back(new Instruction_ior());
      // Pop the result into the resulting variable
      bytecode_block->instructions.push_back(new Instruction_istore(resolve_variable(expression->result)));
      continue;
    }

    const UnaryExpression *unary_expression = dynamic_cast<const UnaryExpression *>(code);
    if (unary_expression != nullptr) {
      push_address(unary_expression->left, bytecode_block);
      if (unary_expression->ir_operator.compare("!") == 0)
        bytecode_block->instructions.push_back(new Instruction_inot());
      continue;
    }

    const Push *push = dynamic_cast<const Push*>(code);
    if (push != nullptr) {
      push_address(push->left, bytecode_block);
      continue;
    }

    const MethodCall *method_call = dynamic_cast<const MethodCall*>(code);
    if (method_call != nullptr) {
      const Variable *possible_print = dynamic_cast<const Variable*>(method_call->left);
      if (possible_print != nullptr && possible_print->identifier.compare("System.out.println") == 0) {
        // Special case for printing
        // Push the number of parameters pushed onto the stack to print
        push_address(method_call->right, bytecode_block);
        bytecode_block->instructions.push_back(new Instruction_print());
      } else {
        // Push method identifier and number of parameters
        push_address(method_call->left, bytecode_block);
        push_address(method_call->right, bytecode_block);
        bytecode_block->instructions.push_back(new Instruction_invokevirtual());
        // Pop result into variable
        bytecode_block->instructions.push_back(new Instruction_istore(resolve_variable(method_call->result)));
      }
      continue;
    }

    const Return *return_ = dynamic_cast<const Return*>(code);
    if (return_ != nullptr) {
      push_address(return_->left, bytecode_block);
      bytecode_block->instructions.push_back(new Instruction_ireturn());
      continue;
    }

    const Copy *copy = dynamic_cast<const Copy*>(code);
    if (copy != nullptr) {
      push_address(copy->left, bytecode_block);
      const Variable *variable = dynamic_cast<const Variable*>(copy->result);
      bytecode_block->instructions.push_back(new Instruction_istore(variable->identifier));
      continue;
    }

    const ConditionalJump *conditional_jump = dynamic_cast<const ConditionalJump*>(code);
    if (conditional_jump != nullptr) {
      // Push condition
      push_address(conditional_jump->left, bytecode_block);
      // Get the target "address" from the constant
      const Constant *target = dynamic_cast<Constant *>(conditional_jump->right);
      bytecode_block->instructions.push_back(new Instruction_iffalse(target->value));
      continue;
    }

    const UnconditionalJump *unconditional_jump = dynamic_cast<const UnconditionalJump*>(code);
    if (unconditional_jump != nullptr) {
      // Get the target "address" from the constant
      const Constant *target = dynamic_cast<Constant *>(unconditional_jump->left);
      bytecode_block->instructions.push_back(new Instruction_goto(target->value));
      continue;
    }

    const Parameter *parameter = dynamic_cast<const Parameter *>(code);
    if (parameter != nullptr) {
      const Variable *variable = dynamic_cast<const Variable *>(parameter->left);
      bytecode_block->instructions.push_back(new Instruction_istore(variable->identifier));
      continue;
    }
  }

  if (block->positive_branch != nullptr)
    generate_block_bytecode(block->positive_branch, bytecode, visited, false);
  if (block->negative_branch != nullptr)
    generate_block_bytecode(block->negative_branch, bytecode, visited, false);
}

void generate_bytecode(std::map<std::string, ControlFlowGraph *> &cfgs, ControlFlowGraph *entry_point, Bytecode *bytecode) {
  // Recursion guard
  std::map<long, bool> visited;
  for (const auto &[_, cfg] : cfgs) {
    generate_block_bytecode(cfg->entry_point, bytecode, visited, cfg == entry_point);
  }
}
