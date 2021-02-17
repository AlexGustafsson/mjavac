#ifndef OPERATOR_H
#define OPERATOR_H

#include <string>

namespace mjavac {
namespace nodes {
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
  Subscript
};

} // namespace nodes
} // namespace mjavac

std::ostream &operator<<(std::ostream &out, mjavac::nodes::Operator binary_operator);

#endif
