#include <iostream>
#include <list>
#include <tuple>

#include "instructions.hpp"

using namespace mjavac::vm;

Instruction_iload::Instruction_iload(std::string identifier)
    : identifier(identifier) {
}

void Instruction_iload::perform(State *state) const {
  state->stack.push(state->variables[this->identifier]);
  state->instruction_pointer++;
}

void Instruction_iload::write(std::ostream &stream) const {
  stream << "iload " << this->identifier << std::endl;
}

Instruction_iconst::Instruction_iconst(long value)
    : value(value) {
}

void Instruction_iconst::perform(State *state) const {
  state->stack.push(this->value);
  state->instruction_pointer++;
}

void Instruction_iconst::write(std::ostream &stream) const {
  stream << "iconst " << this->value << std::endl;
}

Instruction_istore::Instruction_istore(std::string identifier)
    : identifier(identifier) {
}

void Instruction_istore::perform(State *state) const {
  state->variables[this->identifier] = (int)state->stack.top();
  state->stack.pop();
  state->instruction_pointer++;
}

void Instruction_istore::write(std::ostream &stream) const {
  stream << "istore " << this->identifier << std::endl;
}

Instruction_iadd::Instruction_iadd() {
}

void Instruction_iadd::perform(State *state) const {
  int b = (int)state->stack.top();
  state->stack.pop();
  int a = (int)state->stack.top();
  state->stack.pop();
  state->stack.push(a + b);
  state->instruction_pointer++;
}

void Instruction_iadd::write(std::ostream &stream) const {
  stream << "iadd" << std::endl;
}

Instruction_isub::Instruction_isub() {
}

void Instruction_isub::perform(State *state) const {
  int b = (int)state->stack.top();
  state->stack.pop();
  int a = (int)state->stack.top();
  state->stack.pop();
  state->stack.push(a - b);
  state->instruction_pointer++;
}

void Instruction_isub::write(std::ostream &stream) const {
  stream << "isub" << std::endl;
}

Instruction_imul::Instruction_imul() {
}

void Instruction_imul::perform(State *state) const {
  int b = (int)state->stack.top();
  state->stack.pop();
  int a = (int)state->stack.top();
  state->stack.pop();
  state->stack.push(a * b);
  state->instruction_pointer++;
}

void Instruction_imul::write(std::ostream &stream) const {
  stream << "imul" << std::endl;
}

Instruction_idiv::Instruction_idiv() {
}

void Instruction_idiv::perform(State *state) const {
  int b = (int)state->stack.top();
  state->stack.pop();
  int a = (int)state->stack.top();
  state->stack.pop();
  state->stack.push(a / b);
  state->instruction_pointer++;
}

void Instruction_idiv::write(std::ostream &stream) const {
  stream << "idiv" << std::endl;
}

Instruction_ilt::Instruction_ilt() {
}

void Instruction_ilt::perform(State *state) const {
  int b = (int)state->stack.top();
  state->stack.pop();
  int a = (int)state->stack.top();
  state->stack.pop();
  state->stack.push(a < b ? 1 : 0);
  state->instruction_pointer++;
}

void Instruction_ilt::write(std::ostream &stream) const {
  stream << "ilt" << std::endl;
}

Instruction_iand::Instruction_iand() {
}

void Instruction_iand::perform(State *state) const {
  int b = (int)state->stack.top();
  state->stack.pop();
  int a = (int)state->stack.top();
  state->stack.pop();
  state->stack.push(a * b == 0 ? 0 : 1);
  state->instruction_pointer++;
}

void Instruction_iand::write(std::ostream &stream) const {
  stream << "iand" << std::endl;
}

Instruction_ior::Instruction_ior() {
}

void Instruction_ior::perform(State *state) const {
  int b = (int)state->stack.top();
  state->stack.pop();
  int a = (int)state->stack.top();
  state->stack.pop();
  state->stack.push(a + b == 0 ? 0 : 1);
  state->instruction_pointer++;
}

void Instruction_ior::write(std::ostream &stream) const {
  stream << "ior" << std::endl;
}

Instruction_inot::Instruction_inot() {
}

void Instruction_inot::perform(State *state) const {
  int a = (int)state->stack.top();
  state->stack.pop();
  state->stack.push(a == 0 ? 1 : 0);
  state->instruction_pointer++;
}

void Instruction_inot::write(std::ostream &stream) const {
  stream << "inot" << std::endl;
}

Instruction_goto::Instruction_goto(long target)
    : target(target) {
}

void Instruction_goto::perform(State *state) const {
  state->current_block = std::to_string(this->target);
  state->instruction_pointer = 0;
}

void Instruction_goto::write(std::ostream &stream) const {
  stream << "goto " << this->target << std::endl;
}

Instruction_iffalse::Instruction_iffalse(long target)
    : target(target) {
}

void Instruction_iffalse::perform(State *state) const {
  int a = (int)state->stack.top();
  state->stack.pop();
  if (a == 0) {
    state->current_block = std::to_string(this->target);
    state->instruction_pointer = 0;
  } else {
    state->instruction_pointer++;
  }
}

void Instruction_iffalse::write(std::ostream &stream) const {
  stream << "iffalse " << this->target << std::endl;
}

Instruction_invokevirtual::Instruction_invokevirtual(std::string identifier)
    : identifier(identifier) {
}

void Instruction_invokevirtual::perform(State *state) const {
  // Push all the variables to the variable stack
  state->variable_stack.push(state->variables);
  state->variables.clear();

  // Push the current call to the call stack
  state->call_stack.push(std::tuple<std::string, long>(state->current_block, state->instruction_pointer));

  // Change the target block
  state->current_block = this->identifier;
  state->instruction_pointer = 0;

  // Pop the number of parameters passed, as it's not used
  state->stack.pop();
}

void Instruction_invokevirtual::write(std::ostream &stream) const {
  stream << "invokevirtual " << this->identifier << std::endl;
}

Instruction_ireturn::Instruction_ireturn() {
}

void Instruction_ireturn::perform(State *state) const {
  // Pop all the variables from the variable stack
  state->variables = state->variable_stack.top();
  state->variable_stack.pop();

  // Pop the previous call from the call stack
  std::tuple<std::string, long> entry = state->call_stack.top();
  // Change the target block
  state->current_block = std::get<0>(entry);
  state->instruction_pointer = std::get<1>(entry);
  state->call_stack.pop();

  // Increment the instruction pointer by one to move on
  state->instruction_pointer++;
}

void Instruction_ireturn::write(std::ostream &stream) const {
  stream << "ireturn" << std::endl;
}

Instruction_print::Instruction_print() {
}

void Instruction_print::perform(State *state) const {
  int parameter_count = (int)state->stack.top();
  state->stack.pop();

  for (int i = 0; i < parameter_count; i++) {
    int parameter = (int)state->stack.top();
    state->stack.pop();
    std::cout << parameter << std::endl;
  }
  state->instruction_pointer++;
}

void Instruction_print::write(std::ostream &stream) const {
  stream << "print" << std::endl;
}

Instruction_stop::Instruction_stop() {
}

void Instruction_stop::perform(State *state) const {
  // Will end as the block ends after this instruction
  state->instruction_pointer++;
}

void Instruction_stop::write(std::ostream &stream) const {
  stream << "stop" << std::endl;
}
