#ifndef CLASS_DECLERATION_NODE_H
#define CLASS_DECLERATION_NODE_H

#include <fstream>
#include <string>
#include <list>

#include "node.hpp"
#include "variable-declaration-node.hpp"
#include "method-declaration-node.hpp"

namespace nodes {
class ClassDeclarationNode : public Node {
public:
  std::string identifier;
  std::string extended_identifier;
  bool extends;
  std::list<VariableDeclarationNode*> variable_declarations;
  std::list<MethodDeclarationNode*> method_declarations;

  ClassDeclarationNode(std::string identifier, std::string extended_identifier);
  ClassDeclarationNode(std::string identifier);

  void setDeclarations(std::list<Node *> declarations);
  void generate_parse_graph(std::ofstream &stream) const;
};
}

#endif
