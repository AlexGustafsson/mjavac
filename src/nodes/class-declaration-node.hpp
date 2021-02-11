#ifndef CLASS_DECLERATION_NODE_H
#define CLASS_DECLERATION_NODE_H

#include <fstream>
#include <iostream>
#include <list>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "node.hpp"
#include "variable-declaration-node.hpp"
#include "method-declaration-node.hpp"

/* Base node class */
namespace nodes {
class ClassDeclarationNode : public Node {
public:
  std::string identifier;
  std::string extended_identifier;
  bool extends;
  std::list<VariableDeclarationNode*> variable_declarations;
  std::list<MethodDeclarationNode*> method_declarations;

  ClassDeclarationNode(std::string identifier, std::string extended_identifier);
  ClassDeclarationNode(std::string identifier);

  void setDeclarations(std::list<Node*> declaration);
};
} // namespace nodes

#endif
