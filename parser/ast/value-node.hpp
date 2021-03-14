#ifndef VALUE__NODE_H
#define VALUE__NODE_H

#include <list>
#include <ostream>
#include <string>

#include "node.hpp"

namespace mjavac {
namespace ast {
class ValueNode : public Node {
public:
  static int Integer;
  static int Boolean;
  static int Identifier;

  int type;

  int integer_value;
  bool boolean_value;
  std::string identifier_value;

  bool is_array;

  ValueNode(int type, std::string value);

  void generate_parse_graph(std::ostream &stream) const;
};
} // namespace ast
} // namespace mjavac

#endif
