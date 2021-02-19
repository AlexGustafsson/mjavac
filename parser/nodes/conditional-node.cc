#include <ostream>

#include "conditional-node.hpp"
using namespace mjavac::nodes;

ConditionalNode::ConditionalNode(Node *expression, std::list<Node *> statements) {
  this->expression = expression;
  this->statements = statements;
  this->next = nullptr;
}

ConditionalNode::ConditionalNode(Node *expression, Node *statement) {
  this->expression = expression;
  this->statements.push_back(statement);
  this->next = nullptr;
}

ConditionalNode::ConditionalNode(std::list<Node *> statements) {
  this->expression = nullptr;
  this->statements = statements;
  this->next = nullptr;
}

ConditionalNode::ConditionalNode(Node *statement) {
  this->expression = nullptr;;
  this->statements.push_back(statement);
  this->next = nullptr;
}

void ConditionalNode::generate_parse_graph(std::ostream &stream) const {
  if (this->expression == nullptr) {
    stream << this->get_id() << " [label=\"else\"];" << std::endl;
  } else {
    stream << this->get_id() << " [label=\"if\"];" << std::endl;
    stream << this->get_id() << " -> " << this->expression->get_id() << " [label=\"expression\"];" << std::endl;
    this->expression->generate_parse_graph(stream);
  }

  for (const auto& statement : this->statements) {
    stream << this->get_id() << " -> " << statement->get_id() << " [label=\"statement\"];" << std::endl;
    statement->generate_parse_graph(stream);
  }

  if (this->next != nullptr) {
    stream << this->get_id() << " -> " << this->next->get_id() << " [label=\"next\"];" << std::endl;
    this->next->generate_parse_graph(stream);
  }
}
