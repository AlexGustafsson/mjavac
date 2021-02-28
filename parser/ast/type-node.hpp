#ifndef TYPE_NODE_H
#define TYPE_NODE_H

#include <ostream>
#include <string>

#include "node.hpp"

namespace mjavac {
namespace ast {
class TypeNode : public Node {
public:
  std::string type;
  bool is_array;

  TypeNode(std::string type, bool is_array = false);

  void generate_parse_graph(std::ostream &stream) const;
};
} // namespace ast
} // namespace mjavac

#endif
