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
class LoopNode : public Node {
public:
  Node* expression;
  Node *loop_expression;

  LoopNode(Node* expression, Node* loop_expression);
};
} // namespace nodes

#endif
