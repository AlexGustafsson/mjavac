#ifndef BINARY_OPERATION_NODE_H
#define BINARY_OPERATION_NODE_H

#include <ostream>
#include <string>

#include "node.hpp"
#include "operator.hpp"

namespace mjavac {
namespace nodes {
class BinaryOperationNode : public Node {
public:
  Node *left;
  Node *right;
  // Sadly operator is a keyword in this context
  Operator binary_operator;

  BinaryOperationNode(Node *left, Node *right, Operator binary_operator);

  void generate_parse_graph(std::ostream &stream) const;
};
} // namespace nodes
} // namespace mjavac

#endif
