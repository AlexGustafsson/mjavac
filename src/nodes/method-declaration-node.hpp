#ifndef METHOD_DECLERATION_NODE_H
#define METHOD_DECLERATION_NODE_H

#include <fstream>
#include <iostream>
#include <list>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "node.hpp"
#include "variable-declaration-node.hpp"

/* Base node class */
namespace nodes {
class MethodDeclarationNode : public Node {

public:
  bool is_public;
  std::string type;
  std::string identifier;
  std::list<VariableDeclarationNode*> parameters;
  std::list<Node*> statements;

  MethodDeclarationNode(std::string type, std::string identifier);
};
} // namespace nodes

#endif
