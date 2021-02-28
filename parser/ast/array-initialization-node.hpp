#ifndef ARRAY_INITIALIZATION_NODE_H
#define ARRAY_INITIALIZATION_NODE_H

#include <ostream>
#include <string>

#include "node.hpp"
#include "type-node.hpp"

/* Base node class */
namespace mjavac {
namespace ast {
class ArrayInitializationNode : public Node {
public:
  TypeNode *type;
  Node *initialization_expression;

  ArrayInitializationNode(TypeNode *type, Node *initialization_expression);

  void generate_parse_graph(std::ostream &stream) const;
};
} // namespace ast
} // namespace mjavac

#endif
