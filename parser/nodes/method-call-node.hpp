#ifndef METHOD_CALL_NODE_H
#define METHOD_CALL_NODE_H

#include <list>
#include <ostream>

#include "node.hpp"

namespace mjavac {
namespace nodes {
class MethodCallNode : public Node {
public:
  Node *value;
  std::list<Node *> parameters;
  bool is_new;

  MethodCallNode();

  void generate_parse_graph(std::ostream &stream) const;
};
} // namespace nodes
} // namespace mjavac

#endif
