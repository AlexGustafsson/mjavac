#ifndef VALUE__NODE_H
#define VALUE__NODE_H

#include <ostream>
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
  static int Object;

  int type;

  int integer_value;
  bool boolean_value;
  std::string identifier_value;
  std::list<std::string> object_value;

  Node *array_index;
  bool is_array;

  ValueNode(int type, std::string value);
  ValueNode(int type, std::list<std::string> object);

  void generate_parse_graph(std::ostream &stream) const;
};
} // namespace nodes
} // namespace mjavac

#endif
