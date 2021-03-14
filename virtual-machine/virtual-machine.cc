#include <iostream>

#include "virtual-machine.hpp"

using namespace mjavac::vm;

VirtualMachine::VirtualMachine(Bytecode *bytecode) {
  this->instruction_pointer = 0;
  this->bytecode = bytecode;
  this->current_block = bytecode->entry_point;
}

int VirtualMachine::step() {
  if (this->instruction_pointer >= this->current_block->instructions.size())
    return 0;

  Instruction *instruction = this->current_block->instructions[this->instruction_pointer];
  // TODO: temp write the instruction
  instruction->write(std::cerr);

  instruction->perform(this->stack, this->variables);

  this->instruction_pointer++;

  // TODO: Handle all branches
  if (dynamic_cast<Instruction_stop *>(instruction) != nullptr)
    return 0;

  return 1;
}

int VirtualMachine::execute() {
  int instructions = 0;
  while (this->step())
    instructions++;
  return instructions;
}
