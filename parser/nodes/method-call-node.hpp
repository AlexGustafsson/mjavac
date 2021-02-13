#ifndef METHOD_CALL_NODE_H
#define METHOD_CALL_NODE_H

#include <fstream>
#include <list>

#include "node.hpp"

namespace mjavac {
namespace nodes {
class MethodCallNode : public Node {
public:
  std::list<std::string> objects;
  std::list<Node *> parameters;
  bool is_new;

  MethodCallNode();

  void generate_parse_graph(std::ofstream &stream) const;
};
} // namespace nodes
} // namespace mjavac

#endif
