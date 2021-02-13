#ifndef VALUE__NODE_H
#define VALUE__NODE_H

#include <fstream>
#include <list>
#include <string>

#include "node.hpp"

namespace mjavac {
namespace nodes {
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

  ValueNode(int type, std::string value, bool is_array = false);

  void generate_parse_graph(std::ofstream &stream) const;
};
} // namespace nodes
} // namespace mjavac

#endif
