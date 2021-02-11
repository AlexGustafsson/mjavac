#ifndef METHOD_DECLERATION_NODE_H
#define METHOD_DECLERATION_NODE_H

#include <fstream>
#include <list>
#include <string>

#include "node.hpp"
#include "variable-declaration-node.hpp"

namespace nodes {
class MethodDeclarationNode : public Node {
public:
  bool is_public;
  bool is_static;
  std::string type;
  std::string identifier;
  std::list<VariableDeclarationNode *> parameters;
  std::list<Node *> statements;

  MethodDeclarationNode(bool is_public, bool is_static);

  void generate_parse_graph(std::ofstream &stream) const;
};
} // namespace nodes

#endif
