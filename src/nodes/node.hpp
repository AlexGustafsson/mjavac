#ifndef NODE_H
#define NODE_H

#include <fstream>
#include <iostream>
#include <list>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

/* Base node class */
namespace nodes {
class Node {
public:
  Node();

  virtual void generate_parse_graph(std::ofstream &stream);

  intptr_t get_id() const;

  virtual ~Node();
};
}

#endif
