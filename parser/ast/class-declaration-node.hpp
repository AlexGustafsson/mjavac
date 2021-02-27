#ifndef CLASS_DECLERATION_NODE_H
#define CLASS_DECLERATION_NODE_H

#include <list>
#include <ostream>
#include <string>

#include "method-declaration-node.hpp"
#include "node.hpp"
#include "variable-node.hpp"

namespace mjavac {
namespace ast {
class ClassDeclarationNode : public Node {
public:
  std::string identifier;
  std::string extended_identifier;
  bool extends;
  std::list<VariableNode *> variable_declarations;
  std::list<MethodDeclarationNode *> method_declarations;

  ClassDeclarationNode(std::string identifier, std::string extended_identifier);
  ClassDeclarationNode(std::string identifier);

  void setDeclarations(std::list<Node *> declarations);
  void generate_parse_graph(std::ostream &stream) const;
};
} // namespace ast
} // namespace mjavac

#endif
