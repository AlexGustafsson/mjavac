#include <fstream>
#include <iostream>
#include <list>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "node.hpp"
using namespace nodes;

intptr_t Node::get_id() const {
  return reinterpret_cast<intptr_t>(this);
}
