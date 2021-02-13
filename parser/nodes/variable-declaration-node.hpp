#ifndef VARIABLE_DECLERATION_NODE_H
#define VARIABLE_DECLERATION_NODE_H

#include <fstream>
#include <string>

#include "node.hpp"

/* Base node class */
namespace mjavac {
namespace nodes {
class VariableDeclarationNode : public Node {
public:
  std::string type;
  std::string identifier;
  bool is_array;
  Node* value;

  VariableDeclarationNode(std::string type, std::string identifier, bool is_array = false);

  void generate_parse_graph(std::ofstream &stream) const;
};
} // namespace nodes
} // namespace mjavac

#endif
