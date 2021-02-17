#ifndef PROGRAM_NODE_H
#define PROGRAM_NODE_H

#include <ostream>
#include <list>

#include "class-declaration-node.hpp"
#include "node.hpp"

namespace mjavac {
namespace nodes {
class ProgramNode : public Node {
public:
  std::string file_name;
  std::list<ClassDeclarationNode *> declarations;

  ProgramNode(std::string file_name);

  void generate_parse_graph(std::ostream &stream) const;
};
} // namespace nodes
} // namespace mjavac

#endif
