#include "node.hpp"
using namespace mjavac::nodes;

Node::Node() {
  this->location.defined = false;
}

intptr_t Node::get_id() const {
  return reinterpret_cast<intptr_t>(this);
}
