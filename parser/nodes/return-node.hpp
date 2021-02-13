#ifndef RETURN_NODE_NODE_H
#define RETURN_NODE_NODE_H

#include <fstream>

#include "node.hpp"

namespace mjavac {
namespace nodes {
class ReturnNode : public Node {
public:
  Node *value;

  ReturnNode();
  ReturnNode(Node *value);

  void generate_parse_graph(std::ofstream &stream) const;
};
} // namespace nodes
} // namespace mjavac

#endif
