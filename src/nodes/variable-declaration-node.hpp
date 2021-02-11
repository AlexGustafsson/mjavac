#ifndef VARIABLE_DECLERATION_NODE_H
#define VARIABLE_DECLERATION_NODE_H

#include <fstream>
#include <iostream>
#include <list>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "node.hpp"

/* Base node class */
namespace nodes {
class VariableDeclarationNode : public Node {

public:
  std::string type;
  std::string identifier;
  bool is_array;

  VariableDeclarationNode(std::string type, std::string identifier, bool is_array=false);
};
} // namespace nodes

#endif
