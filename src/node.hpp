#ifndef NODE_H
#define NODE_H

#include <fstream>
#include <iostream>
#include <list>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

class Node {
public:
  int id;
  std::string type;
  std::string value;
  std::list<Node *> children;

  Node();
  Node(std::string type, std::string value);

  void print_tree(int depth);
  void generate_tree(int &count, std::ofstream *outStream);
};

#endif
