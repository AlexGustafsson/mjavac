#ifndef CONDITIONAL_NODE_H
#define CONDITIONAL_NODE_H

#include <fstream>

#include "node.hpp"

namespace nodes {
class ConditionalNode : public Node {
public:
  Node* expression;
  Node* positive_expression;
  Node* negative_expression;

  ConditionalNode(Node* expression, Node* positive_expression, Node* negative_expression);

  void generate_parse_graph(std::ofstream &stream) const;
};
}

#endif
