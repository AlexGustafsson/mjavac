#include <iostream>

#include "virtual-machine.hpp"

using namespace mjavac::vm;

VirtualMachine::VirtualMachine(Bytecode *bytecode) {
  this->bytecode = bytecode;
  this->state = new State(bytecode->entry_point->identifier);
}

int VirtualMachine::step() {
  if (this->state->instruction_pointer >= this->bytecode->blocks[this->state->current_block]->instructions.size())
    return 0;

  Instruction *instruction = this->bytecode->blocks[this->state->current_block]->instructions[this->state->instruction_pointer];
  // Write out each instruction
  // instruction->write(std::cerr);

  instruction->perform(this->state);

  return 1;
}

int VirtualMachine::execute() {
  int instructions = 0;
  while (true) {
    if (this->step()) {
      instructions++;
    } else {
      if (this->state->call_stack.size() == 0) {
        break;
      } else {
        // Handle implicit void returns
        // Pop all the variables from the variable stack
        this->state->variables = this->state->variable_stack.top();
        this->state->variable_stack.pop();

        // Pop the previous call from the call stack
        std::tuple<std::string, long> entry = this->state->call_stack.top();
        // Change the target block
        this->state->current_block = std::get<0>(entry);
        this->state->instruction_pointer = std::get<1>(entry);
        this->state->call_stack.pop();

        // Push an empty value onto the stack
        this->state->stack.push(0);

        // Increment the instruction pointer by one to move on
        this->state->instruction_pointer++;
      }
    }
  }
  return instructions;
}
