#ifndef LOOP_NODE_H
#define LOOP_NODE_H

#include <fstream>
#include <list>

#include "node.hpp"

namespace mjavac {
namespace nodes {
class LoopNode : public Node {
public:
  Node *expression;
  std::list<Node *> statements;

  LoopNode(Node *statement);

  void generate_parse_graph(std::ofstream &stream) const;
};
} // namespace nodes
} // namespace mjavac

#endif
