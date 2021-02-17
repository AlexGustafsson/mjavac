#ifndef METHOD_DECLERATION_NODE_H
#define METHOD_DECLERATION_NODE_H

#include <ostream>
#include <list>
#include <string>

#include "node.hpp"
#include "variable-node.hpp"

namespace mjavac {
namespace nodes {
class MethodDeclarationNode : public Node {
public:
  bool is_public;
  bool is_static;
  std::string type;
  std::string identifier;
  std::list<VariableNode *> parameters;
  std::list<Node *> statements;

  MethodDeclarationNode(bool is_public, bool is_static);

  void generate_parse_graph(std::ostream &stream) const;
};
} // namespace nodes
} // namespace mjavac

#endif
