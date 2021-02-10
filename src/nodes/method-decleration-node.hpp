#ifndef METHOD_DECLERATION_NODE_H
#define METHOD_DECLERATION_NODE_H

#include <fstream>
#include <iostream>
#include <list>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "node.hpp"
#include "variable-decleration-node.hpp"

/* Base node class */
namespace nodes {
class MethodDeclerationNode : public Node {

public:
  bool is_public;
  std::string type;
  std::string identifier;
  std::list<VariableDeclerationNode*> parameters;
  std::list<Node*> statements;

  MethodDeclerationNode(std::string type, std::string identifier);
};
} // namespace nodes

#endif
