#ifndef PROGRAM_NODE_H
#define PROGRAM_NODE_H

#include <list>
#include <ostream>

#include "../source.hpp"
#include "class-declaration-node.hpp"

#include "node.hpp"

namespace mjavac {
namespace ast {
class ProgramNode : public Node {
public:
  Source *source;
  std::list<ClassDeclarationNode *> declarations;

  ProgramNode(Source *source);

  void generate_parse_graph(std::ostream &stream) const;
};
} // namespace ast
} // namespace mjavac

#endif
