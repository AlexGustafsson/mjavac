#ifndef VARIABLE_NODE_H
#define VARIABLE_NODE_H

#include <ostream>
#include <string>

#include "node.hpp"
#include "type-node.hpp"

/* Base node class */
namespace mjavac {
namespace ast {
class VariableNode : public Node {
public:
  TypeNode *type;
  std::string identifier;

  VariableNode(TypeNode *type, std::string identifier);

  void generate_parse_graph(std::ostream &stream) const;
};
} // namespace ast
} // namespace mjavac

#endif
