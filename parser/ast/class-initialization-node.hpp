#ifndef CLASS_INITIALIZATION_NODE_H
#define CLASS_INITIALIZATION_NODE_H

#include <ostream>
#include <string>

#include "node.hpp"
#include "type-node.hpp"

/* Base node class */
namespace mjavac {
namespace ast {
class ClassInitializationNode : public Node {
public:
  std::string identifier;

  ClassInitializationNode(std::string identifier);

  void generate_parse_graph(std::ostream &stream) const;
};
} // namespace ast
} // namespace mjavac

#endif
