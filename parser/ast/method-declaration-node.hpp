#ifndef METHOD_DECLERATION_NODE_H
#define METHOD_DECLERATION_NODE_H

#include <list>
#include <ostream>
#include <string>

#include "node.hpp"
#include "variable-node.hpp"
#include "type-node.hpp"

namespace mjavac {
namespace ast {
class MethodDeclarationNode : public Node {
public:
  bool is_public;
  bool is_static;
  TypeNode* type;
  std::string identifier;
  std::list<VariableNode *> parameters;
  std::list<Node *> statements;

  MethodDeclarationNode(bool is_public, bool is_static);

  void generate_parse_graph(std::ostream &stream) const;
};
} // namespace ast
} // namespace mjavac

#endif
