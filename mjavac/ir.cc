#include "ir.hpp"

using namespace mjavac::ir;

std::ostream &operator<<(std::ostream &stream, const Address *address) {
  if (address == nullptr)
    stream << "null";
  else
    address->write(stream);
  return stream;
}

Variable::Variable(std::string identifier)
    : identifier(identifier) {
}

void Variable::write(std::ostream &stream) const {
  stream << this->identifier;
}

Constant::Constant(int value)
    : value(value) {
}

void Constant::write(std::ostream &stream) const {
  stream << "$" << this->value;
}

TemporaryVariable::TemporaryVariable(unsigned long long id)
    : id(id) {
}

void TemporaryVariable::write(std::ostream &stream) const {
  stream << "_t" << this->id;
}

ThreeAddressCode::ThreeAddressCode(Address *left, Address *right)
    : result(nullptr), left(left), right(right) {
}

Expression::Expression(Address *left, Address *right, std::string ir_operator)
    : ThreeAddressCode(left, right), ir_operator(ir_operator) {
}

void Expression::write(std::ostream &stream) const {
  stream << this->result << " := " << this->left << " " << this->ir_operator << " " << this->right;
}

UnaryExpression::UnaryExpression(Address *operand, std::string ir_operator)
    : ThreeAddressCode(operand, nullptr), ir_operator(ir_operator) {
}

void UnaryExpression::write(std::ostream &stream) const {
  stream << this->result << " := " << this->ir_operator << " " << this->left;
}

Copy::Copy(Address *operand)
    : ThreeAddressCode(operand, nullptr) {
}

void Copy::write(std::ostream &stream) const {
  stream << this->result << " := " << this->left;
}

ArrayAccess::ArrayAccess(Address *left, Address *right)
    : ThreeAddressCode(left, right) {
}

void ArrayAccess::write(std::ostream &stream) const {
  stream << this->result << " := " << this->left << "[" << this->right << "]";
}

New::New(Address *operand)
    : ThreeAddressCode(operand, nullptr) {
}

void New::write(std::ostream &stream) const {
  stream << this->result << " := new " << this->left;
}

NewArray::NewArray(Address *left, Address *right)
    : ThreeAddressCode(left, right) {
}

void NewArray::write(std::ostream &stream) const {
  stream << this->result << " := new " << this->left << "[" << this->right << "]";
}

Push::Push(Address *operand)
    : ThreeAddressCode(operand, nullptr) {
}

void Push::write(std::ostream &stream) const {
  stream << this->result << " := push " << this->left;
}

Parameter::Parameter(Address *operand)
    : ThreeAddressCode(operand, nullptr) {
}

void Parameter::write(std::ostream &stream) const {
  stream << this->result << " := param " << this->left;
}

MethodCall::MethodCall(Address *left, Address *right)
    : ThreeAddressCode(left, right) {
}

void MethodCall::write(std::ostream &stream) const {
  stream << this->result << " := call " << this->left << "," << this->right;
}

Return::Return(Address *operand)
    : ThreeAddressCode(operand, nullptr) {
}

void Return::write(std::ostream &stream) const {
  stream << this->result << " := return " << this->left;
}

UnconditionalJump::UnconditionalJump(Address *operand)
    : ThreeAddressCode(operand, nullptr) {
}

void UnconditionalJump::write(std::ostream &stream) const {
  stream << this->result << " := goto " << this->left;
}

ConditionalJump::ConditionalJump(Address *left, Address *right)
    : ThreeAddressCode(left, right) {
}

void ConditionalJump::write(std::ostream &stream) const {
  stream << this->result << " := iffalse " << this->left << " goto " << this->right;
}
