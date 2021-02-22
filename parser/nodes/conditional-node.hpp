#ifndef CONDITIONAL_NODE_H
#define CONDITIONAL_NODE_H

#include <list>
#include <ostream>

#include "node.hpp"

namespace mjavac {
namespace nodes {
class ConditionalNode : public Node {
public:
  Node *expression;
  std::list<Node *> statements;
  ConditionalNode *next;

  ConditionalNode(Node *expression, std::list<Node *> statements);
  ConditionalNode(Node *expression, Node *statement);

  ConditionalNode(std::list<Node *> statements);
  ConditionalNode(Node *statement);

  void generate_parse_graph(std::ostream &stream) const;
};
} // namespace nodes
} // namespace mjavac

#endif
