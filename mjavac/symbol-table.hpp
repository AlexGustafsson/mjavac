#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <bitset>
#include <cstdint>
#include <fstream>
#include <map>

#include <mjavac/nodes/node.hpp>

struct Symbol {
  // ID of the node representing the scope (program, class, method etc.)
  intptr_t scope;
  std::bitset<4> traits;
  mjavac::nodes::Node *node;
};

enum SymbolTrait {
  IntLike = 1,
  Callable,
  StringLike,
  Subscriptable
};

class SymbolTable {
public:
  std::map<intptr_t, Symbol *> symbols;
  SymbolTable();

  void write(std::ofstream &stream) const;
};

#endif
