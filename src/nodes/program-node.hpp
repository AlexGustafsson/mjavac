#ifndef PROGRAM_NODE_H
#define PROGRAM_NODE_H

#include <fstream>
#include <list>

#include "node.hpp"
#include "class-declaration-node.hpp"

namespace nodes {
class ProgramNode : public Node {
public:
  std::list<ClassDeclarationNode*> declarations;

  ProgramNode();

  void generate_parse_graph(std::ofstream &stream) const;
};
}

#endif
