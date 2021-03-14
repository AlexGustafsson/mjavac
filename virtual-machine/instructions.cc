#include <iostream>

#include "instructions.hpp"

using namespace mjavac::vm;

Instruction_iload::Instruction_iload(std::string identifier)
    : identifier(identifier) {
}

void Instruction_iload::perform(std::stack<long> &stack, std::map<std::string, int> &variables) const {
  stack.push(variables[this->identifier]);
}

void Instruction_iload::write(std::ostream &stream) const {
  stream << "iload " << this->identifier << std::endl;
}

Instruction_iconst::Instruction_iconst(long value)
    : value(value) {
}

void Instruction_iconst::perform(std::stack<long> &stack, std::map<std::string, int> &variables) const {
  stack.push(this->value);
}

void Instruction_iconst::write(std::ostream &stream) const {
  stream << "iconst " << this->value << std::endl;
}

Instruction_istore::Instruction_istore(std::string identifier)
    : identifier(identifier) {
}

void Instruction_istore::perform(std::stack<long> &stack, std::map<std::string, int> &variables) const {
  variables[this->identifier] = (int)stack.top();
  stack.pop();
}

void Instruction_istore::write(std::ostream &stream) const {
  stream << "istore " << this->identifier << std::endl;
}

Instruction_iadd::Instruction_iadd() {
}

void Instruction_iadd::perform(std::stack<long> &stack, std::map<std::string, int> &variables) const {
  int a = (int)stack.top();
  stack.pop();
  int b = (int)stack.top();
  stack.pop();
  stack.push(a + b);
}

void Instruction_iadd::write(std::ostream &stream) const {
  stream << "iadd" << std::endl;
}

Instruction_isub::Instruction_isub() {
}

void Instruction_isub::perform(std::stack<long> &stack, std::map<std::string, int> &variables) const {
  int a = (int)stack.top();
  stack.pop();
  int b = (int)stack.top();
  stack.pop();
  stack.push(a - b);
}

void Instruction_isub::write(std::ostream &stream) const {
  stream << "isub" << std::endl;
}

Instruction_imul::Instruction_imul() {
}

void Instruction_imul::perform(std::stack<long> &stack, std::map<std::string, int> &variables) const {
  int a = (int)stack.top();
  stack.pop();
  int b = (int)stack.top();
  stack.pop();
  stack.push(a * b);
}

void Instruction_imul::write(std::ostream &stream) const {
  stream << "imul" << std::endl;
}

Instruction_idiv::Instruction_idiv() {
}

void Instruction_idiv::perform(std::stack<long> &stack, std::map<std::string, int> &variables) const {
  int a = (int)stack.top();
  stack.pop();
  int b = (int)stack.top();
  stack.pop();
  stack.push(a / b);
}

void Instruction_idiv::write(std::ostream &stream) const {
  stream << "idiv" << std::endl;
}

Instruction_ilt::Instruction_ilt() {
}

void Instruction_ilt::perform(std::stack<long> &stack, std::map<std::string, int> &variables) const {
  int a = (int)stack.top();
  stack.pop();
  int b = (int)stack.top();
  stack.pop();
  stack.push(a < b ? 1 : 0);
}

void Instruction_ilt::write(std::ostream &stream) const {
  stream << "ilt" << std::endl;
}

Instruction_iand::Instruction_iand() {
}

void Instruction_iand::perform(std::stack<long> &stack, std::map<std::string, int> &variables) const {
  int a = (int)stack.top();
  stack.pop();
  int b = (int)stack.top();
  stack.pop();
  stack.push(a * b == 0 ? 0 : 1);
}

void Instruction_iand::write(std::ostream &stream) const {
  stream << "iand" << std::endl;
}

Instruction_ior::Instruction_ior() {
}

void Instruction_ior::perform(std::stack<long> &stack, std::map<std::string, int> &variables) const {
  int a = (int)stack.top();
  stack.pop();
  int b = (int)stack.top();
  stack.pop();
  stack.push(a + b == 0 ? 0 : 1);
}

void Instruction_ior::write(std::ostream &stream) const {
  stream << "ior" << std::endl;
}

Instruction_inot::Instruction_inot() {
}

void Instruction_inot::perform(std::stack<long> &stack, std::map<std::string, int> &variables) const {
  int a = (int)stack.top();
  stack.pop();
  stack.push(a == 0 ? 1 : 0);
}

void Instruction_inot::write(std::ostream &stream) const {
  stream << "inot" << std::endl;
}

Instruction_goto::Instruction_goto(long target)
    : target(target) {
}

void Instruction_goto::perform(std::stack<long> &stack, std::map<std::string, int> &variables) const {
  // TODO
}

void Instruction_goto::write(std::ostream &stream) const {
  stream << "goto" << std::endl;
}

Instruction_iffalse::Instruction_iffalse(long target)
    : target(target) {
}

void Instruction_iffalse::perform(std::stack<long> &stack, std::map<std::string, int> &variables) const {
  int a = (int)stack.top();
  stack.pop();
  if (a == 0) {
    // TODO
  }
}

void Instruction_iffalse::write(std::ostream &stream) const {
  stream << "iffalse" << std::endl;
}

Instruction_invokevirtual::Instruction_invokevirtual(std::string identifier)
    : identifier(identifier) {
}

void Instruction_invokevirtual::perform(std::stack<long> &stack, std::map<std::string, int> &variables) const {
  // TODO
}

void Instruction_invokevirtual::write(std::ostream &stream) const {
  stream << "invokevirtual" << std::endl;
}

Instruction_ireturn::Instruction_ireturn() {
}

void Instruction_ireturn::perform(std::stack<long> &stack, std::map<std::string, int> &variables) const {
  // TODO
}

void Instruction_ireturn::write(std::ostream &stream) const {
  stream << "ireturn" << std::endl;
}

Instruction_print::Instruction_print() {
}

void Instruction_print::perform(std::stack<long> &stack, std::map<std::string, int> &variables) const {
  int value = (int)stack.top();
  std::cout << value << std::endl;
  stack.pop();
}

void Instruction_print::write(std::ostream &stream) const {
  stream << "print" << std::endl;
}

Instruction_stop::Instruction_stop() {
}

void Instruction_stop::perform(std::stack<long> &stack, std::map<std::string, int> &variables) const {
  // TODO
}

void Instruction_stop::write(std::ostream &stream) const {
  stream << "stop" << std::endl;
}
