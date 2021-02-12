#ifndef OPERATOR_H
#define OPERATOR_H

#include <string>

namespace mjavac {
namespace nodes {
struct Operator {
public:
  static std::string And;
  static std::string LessThan;
  static std::string LessThanOrEqual;
  static std::string GreaterThan;
  static std::string GreaterThanOrEqual;
  static std::string Plus;
  static std::string Multiplication;
};
} // namespace nodes
} // namespace mjavac

#endif
