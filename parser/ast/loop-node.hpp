#ifndef LOOP_NODE_H
#define LOOP_NODE_H

#include <list>
#include <ostream>

#include "node.hpp"

namespace mjavac {
namespace ast {
class LoopNode : public Node {
public:
  Node *expression;
  std::list<Node *> statements;

  LoopNode(Node *statement);

  void generate_parse_graph(std::ostream &stream) const;
};
} // namespace ast
} // namespace mjavac

#endif
