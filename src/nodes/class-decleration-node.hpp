#ifndef CLASS_DECLERATION_NODE_H
#define CLASS_DECLERATION_NODE_H

#include <fstream>
#include <iostream>
#include <list>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "node.hpp"
#include "variable-decleration-node.hpp"
#include "method-decleration-node.hpp"

/* Base node class */
namespace nodes {
class ClassDeclerationNode : public Node {
public:
  std::string identifier;
  std::string extended_identifier;
  bool extends;
  std::list<VariableDeclerationNode*> variable_declerations;
  std::list<MethodDeclerationNode*> method_declerations;

  ClassDeclerationNode(std::string identifier, std::string extended_identifier);
  ClassDeclerationNode(std::string identifier);
};
} // namespace nodes

#endif
