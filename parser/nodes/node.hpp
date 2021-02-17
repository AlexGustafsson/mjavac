#ifndef NODE_H
#define NODE_H

#include <ostream>

// 1-based coordinates
struct node_location_t {
  int start_line;
  int start_column;
  int end_line;
  int end_column;
  bool defined;
};

namespace mjavac {
namespace nodes {
class Node {
public:
  node_location_t location;

  Node();

  virtual void generate_parse_graph(std::ostream &stream) const = 0;

  intptr_t get_id() const;
};
} // namespace nodes
} // namespace mjavac

#endif
