#ifndef VARIABLE_NODE_H
#define VARIABLE_NODE_H

#include <fstream>
#include <string>

#include "node.hpp"

/* Base node class */
namespace mjavac {
namespace nodes {
class VariableNode : public Node {
public:
  std::string type;
  std::string identifier;
  bool is_array;
  Node *assigned_value;
  bool is_declaration;

  VariableNode(std::string type, std::string identifier, bool is_declaration, bool is_array = false);

  void generate_parse_graph(std::ofstream &stream) const;
};
} // namespace nodes
} // namespace mjavac

#endif
