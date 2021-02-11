#ifndef CONDITIONAL_NODE_H
#define CONDITIONAL_NODE_H

#include <fstream>

#include "node.hpp"

namespace nodes {
class LoopNode : public Node {
public:
  Node *expression;
  Node *loop_expression;

  LoopNode(Node *expression, Node *loop_expression);

  void generate_parse_graph(std::ofstream &stream) const;
};
} // namespace nodes

#endif
