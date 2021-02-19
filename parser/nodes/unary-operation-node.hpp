#ifndef UNARY_OPERATION_NODE_H
#define UNARY_OPERATION_NODE_H

#include <ostream>
#include <string>

#include "node.hpp"
#include "operator.hpp"

namespace mjavac {
namespace nodes {
class UnaryOperationNode : public Node {
public:
  Node *operand;
  // Sadly operator is a keyword in this context
  Operator unary_operator;

  UnaryOperationNode(Node *operand, Operator unary_operator);

  void generate_parse_graph(std::ostream &stream) const;
};
} // namespace nodes
} // namespace mjavac

#endif
