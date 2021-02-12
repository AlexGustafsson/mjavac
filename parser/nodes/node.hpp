#ifndef NODE_H
#define NODE_H

#include <fstream>

namespace mjavac {
namespace nodes {
class Node {
public:
  Node();

  virtual void generate_parse_graph(std::ofstream &stream) const = 0;

  intptr_t get_id() const;
};
} // namespace nodes
} // namespace mjavac

#endif
