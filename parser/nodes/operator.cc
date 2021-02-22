#include <ostream>

#include "operator.hpp"
using namespace mjavac::nodes;

std::ostream &operator<<(std::ostream &out, Operator binary_operator) {
  switch (binary_operator) {
  case Operator::And:
    out << "&&";
    break;
  case Operator::Assign:
    out << "=";
    break;
  case Operator::Division:
    out << "/";
    break;
  case Operator::Dot:
    out << ".";
    break;
  case Operator::Equal:
    out << "==";
    break;
  case Operator::GreaterThan:
    out << ">";
    break;
  case Operator::GreaterThanOrEqual:
    out << ">=";
    break;
  case Operator::LessThan:
    out << "<";
    break;
  case Operator::LessThanOrEqual:
    out << "<=";
    break;
  case Operator::NotEqual:
    out << "!=";
    break;
  case Operator::Minus:
    out << "-";
    break;
  case Operator::Multiplication:
    out << "*";
    break;
  case Operator::Or:
    out << "||";
    break;
  case Operator::Plus:
    out << "+";
    break;
  case Operator::Subscript:
    out << "[]";
    break;
  case Operator::Negate:
    out << "!";
    break;
  case Operator::Negative:
    out << "-";
    break;
  }

  return out;
}
