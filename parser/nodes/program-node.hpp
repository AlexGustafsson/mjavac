#ifndef PROGRAM_NODE_H
#define PROGRAM_NODE_H

#include <fstream>
#include <list>

#include "class-declaration-node.hpp"
#include "node.hpp"

namespace mjavac {
namespace nodes {
class ProgramNode : public Node {
public:
  std::list<ClassDeclarationNode *> declarations;

  ProgramNode();

  void generate_parse_graph(std::ofstream &stream) const;
};
} // namespace nodes
} // namespace mjavac

#endif
