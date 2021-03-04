#ifndef IR_H
#define IR_H

#include <ostream>
#include <string>

namespace mjavac {
namespace ir {
class Address {
public:
  virtual void write(std::ostream &stream) const = 0;
};

class Variable : public Address {
public:
  std::string identifier;

  Variable(std::string identifier);
  void write(std::ostream &stream) const;
};

class Constant : public Address {
public:
  int value;

  Constant(int value);
  void write(std::ostream &stream) const;
};

class TemporaryVariable : public Address {
public:
  unsigned long long id;

  TemporaryVariable(unsigned long long id);
  void write(std::ostream &stream) const;
};

class ThreeAddressCode {
public:
  Address *result;
  Address *left;
  Address *right;

  ThreeAddressCode(Address *left, Address *right);

  virtual void write(std::ostream &stream) const = 0;
};

class Expression : public ThreeAddressCode {
public:
  std::string ir_operator;

  Expression(Address *left, Address *right, std::string ir_operator);
  void write(std::ostream &stream) const;
};

class UnaryExpression : public ThreeAddressCode {
public:
  std::string ir_operator;

  UnaryExpression(Address *left, std::string ir_operator);
  void write(std::ostream &stream) const;
};

class Copy : public ThreeAddressCode {
public:
  Copy(Address *operand);
  void write(std::ostream &stream) const;
};

class ArrayAccess : public ThreeAddressCode {
public:
  ArrayAccess(Address *left, Address *right);
  void write(std::ostream &stream) const;
};

class New : public ThreeAddressCode {
public:
  New(Address *operand);
  void write(std::ostream &stream) const;
};

class NewArray : public ThreeAddressCode {
public:
  NewArray(Address *left, Address *right);
  void write(std::ostream &stream) const;
};

class Push : public ThreeAddressCode {
public:
  Push(Address *operand);
  void write(std::ostream &stream) const;
};

class Parameter : public ThreeAddressCode {
public:
  Parameter(Address *operand);
  void write(std::ostream &stream) const;
};

class MethodCall : public ThreeAddressCode {
public:
  MethodCall(Address *left, Address *right);
  void write(std::ostream &stream) const;
};

class Return : public ThreeAddressCode {
public:
  Return(Address *operand);
  void write(std::ostream &stream) const;
};

class UnconditionalJump : public ThreeAddressCode {
public:
  UnconditionalJump(Address *target);
  void write(std::ostream &stream) const;
};

class ConditionalJump : public ThreeAddressCode {
public:
  ConditionalJump(Address *condition, Address *target);
  void write(std::ostream &stream) const;
};
} // namespace ir
} // namespace mjavac

#endif
