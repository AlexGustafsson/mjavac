#ifndef CONDITIONAL_NODE_H
#define CONDITIONAL_NODE_H

#include <fstream>
#include <iostream>
#include <list>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "node.hpp"

/* Base node class */
namespace nodes {
class ConditionalNode : public Node {
public:
  Node* expression;
  Node* positive_expression;
  Node* negative_expression;

  ConditionalNode(Node* expression, Node* positive_expression, Node* negative_expression);
};
} // namespace nodes

#endif
