#include "class-declaration-node.hpp"
#include "nodes/method-declaration-node.hpp"
using namespace mjavac::nodes;

ClassDeclarationNode::ClassDeclarationNode(std::string identifier) {
  this->identifier = identifier;
  this->extends = false;
}

ClassDeclarationNode::ClassDeclarationNode(std::string identifier, std::string extended_identifier)
    : ClassDeclarationNode::ClassDeclarationNode(identifier) {
  this->extended_identifier = extended_identifier;
  this->extends = true;
}

void ClassDeclarationNode::setDeclarations(std::list<Node *> declarations) {
  for (const auto &node : declarations) {
    MethodDeclarationNode *method = dynamic_cast<MethodDeclarationNode *>(node);
    if (method != nullptr) {
      this->method_declarations.push_back(method);
      continue;
    }

    VariableNode *variable = dynamic_cast<VariableNode *>(node);
    if (variable != nullptr) {
      this->variable_declarations.push_back(variable);
      continue;
    }

    // ...
  }
}

void ClassDeclarationNode::generate_parse_graph(std::ostream &stream) const {
  stream << this->get_id() << " [label=\"class " << this->identifier;
  if (this->extends)
    stream << " extends " << this->extended_identifier;
  stream << "\"];" << std::endl;

  for (const auto &node : this->variable_declarations) {
    stream << this->get_id() << " -> " << node->get_id() << "[label=\"variable\"];" << std::endl;
    node->generate_parse_graph(stream);
  }

  for (const auto &node : this->method_declarations) {
    stream << this->get_id() << " -> " << node->get_id() << "[label=\"method\"];" << std::endl;
    node->generate_parse_graph(stream);
  }
}
