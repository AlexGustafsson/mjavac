#include <fstream>
#include <iostream>
#include <list>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "node.hpp"

Node::Node(std::string type, std::string value)
    : type(type), value(value) {
}

Node::Node() {
  type = nullptr;
  value = nullptr;
}

void Node::print_tree(int depth = 0) {
  for (int i = 0; i < depth; i++)
    std::cout << "  ";
  std::cout << type << ":" << value << std::endl;
  for (Node *child : this->children)
    child->print_tree(depth + 1);
}

void Node::generate_tree(int &count, std::ofstream *outStream) {
  id = count++;
  *outStream << "n" << id << " [label=\"" << type << ":" << value << "\"];" << std::endl;

  for (Node *child : this->children) {
    child->generate_tree(count, outStream);
    *outStream << "n" << id << " -> n" << child->id << std::endl;
  }
}
