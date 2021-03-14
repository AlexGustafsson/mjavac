#include "ir.hpp"

using namespace mjavac::ir;

std::ostream &operator<<(std::ostream &stream, const Address *address) {
  if (address == nullptr)
    stream << "null";
  else
    address->write(stream);
  return stream;
}

intptr_t Address::get_id() const {
  return reinterpret_cast<intptr_t>(this);
}

Variable::Variable(std::string identifier)
    : identifier(identifier) {
}

void Variable::write(std::ostream &stream) const {
  stream << this->identifier;
}

Constant::Constant(long value)
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

ThreeAddressCode::ThreeAddressCode(Address *result, Address *left, Address *right)
    : result(result), left(left), right(right) {
}

Expression::Expression(Address *result, Address *left, Address *right, std::string ir_operator)
    : ThreeAddressCode(result, left, right), ir_operator(ir_operator) {
}

Expression::Expression(Address *left, Address *right, std::string ir_operator)
    : Expression(nullptr, left, right, ir_operator) {
}

void Expression::write(std::ostream &stream) const {
  stream << this->result << " := " << this->left << " " << this->ir_operator << " " << this->right;
}

UnaryExpression::UnaryExpression(Address *result, Address *operand, std::string ir_operator)
    : ThreeAddressCode(result, operand, nullptr), ir_operator(ir_operator) {
}
UnaryExpression::UnaryExpression(Address *operand, std::string ir_operator)
    : UnaryExpression(nullptr, operand, ir_operator) {
}

void UnaryExpression::write(std::ostream &stream) const {
  stream << this->result << " := " << this->ir_operator << " " << this->left;
}

Copy::Copy(Address *operand)
    : ThreeAddressCode(operand, nullptr) {
}

Copy::Copy(Address *target, Address *operand)
    : ThreeAddressCode(target, operand, nullptr) {
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
  stream << "push " << this->left;
}

Parameter::Parameter(Address *operand)
    : ThreeAddressCode(operand, nullptr) {
}

void Parameter::write(std::ostream &stream) const {
  stream << "param " << this->left;
}

MethodCall::MethodCall(Address *result, Address *left, Address *right)
    : ThreeAddressCode(result, left, right) {
}

MethodCall::MethodCall(Address *left, Address *right)
    : MethodCall(nullptr, left, right) {
}

void MethodCall::write(std::ostream &stream) const {
  if (this->result == nullptr)
    stream << "call " << this->left << ", " << this->right;
  else
    stream << this->result << " := call " << this->left << ", " << this->right;
}

Return::Return(Address *operand)
    : ThreeAddressCode(operand, nullptr) {
}

Return::Return()
    : Return(nullptr) {
}

void Return::write(std::ostream &stream) const {
  stream << "return " << this->left;
}

UnconditionalJump::UnconditionalJump(Address *operand)
    : ThreeAddressCode(operand, nullptr) {
}

void UnconditionalJump::write(std::ostream &stream) const {
  stream << "goto " << this->left;
}

ConditionalJump::ConditionalJump(Address *condition, Address *target)
    : ThreeAddressCode(condition, target) {
}

void ConditionalJump::write(std::ostream &stream) const {
  stream << "iffalse " << this->left << " goto " << this->right;
}
