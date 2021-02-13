#ifndef BINARY_OPERATION_NODE_H
#define BINARY_OPERATION_NODE_H

#include <fstream>
#include <string>

#include "node.hpp"

namespace mjavac {
namespace nodes {
class BinaryOperationNode : public Node {
public:
  Node *left;
  Node *right;
  // Sadly operator is a keyword in this context
  std::string binary_operator;

  BinaryOperationNode(Node *left, Node *right, std::string binary_operator);

  void generate_parse_graph(std::ofstream &stream) const;
};
} // namespace nodes
} // namespace mjavac

#endif
