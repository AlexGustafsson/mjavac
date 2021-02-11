#include "node.hpp"
using namespace nodes;

Node::Node() {}

intptr_t Node::get_id() const {
  return reinterpret_cast<intptr_t>(this);
}
