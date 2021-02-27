#ifndef OPERATOR_H
#define OPERATOR_H

#include <string>

namespace mjavac {
namespace ast {
enum Operator {
  And,
  LessThan,
  LessThanOrEqual,
  GreaterThan,
  GreaterThanOrEqual,
  Plus,
  Minus,
  Multiplication,
  Assign,
  Equal,
  NotEqual,
  Division,
  Or,
  Dot,
  Subscript,
  Negate,
  Negative
};

} // namespace ast
} // namespace mjavac

std::ostream &operator<<(std::ostream &out, mjavac::ast::Operator binary_operator);

#endif
