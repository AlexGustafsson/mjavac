#ifndef PROGRAM_NODE_H
#define PROGRAM_NODE_H

#include <fstream>
#include <iostream>
#include <list>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "node.hpp"
#include "class-declaration-node.hpp"

/* Base node class */
namespace nodes {
class ProgramNode : public Node {
public:
  std::list<ClassDeclarationNode*> declarations;

  ProgramNode();
};
} // namespace nodes

#endif
