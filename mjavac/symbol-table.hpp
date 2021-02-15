#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <bitset>
#include <cstdint>
#include <fstream>
#include <map>

#include <mjavac/nodes/node.hpp>

#define SYMBOL_TRAITS 128

// TODO: Add support for parameters etc. too?
// will scale okay as there aren't too many cases to handle.
// A more generic approach would likely require to implement the
// symbol handling in the AST to be maintainable, but that would
// mean that the parser would be less isolated.
struct Symbol {
  // ID of the node representing the scope (program, class, method etc.)
  intptr_t scope;
  // Traits as specified by SymbolTrait
  int traits;
  // The node itself
  const mjavac::nodes::Node *node;
  // The (optional) name of the symbol, such as the class, method or variable name
  std::string name;

  Symbol(const mjavac::nodes::Node *node, intptr_t scope, int traits) : scope(scope), traits(traits), node(node) {}
  Symbol(const mjavac::nodes::Node *node, std::string name, intptr_t scope, int traits) : scope(scope), traits(traits), node(node), name(name) {}
};

enum SymbolTrait {
  None = 1 << 0,
  // The symbol can be treated as an int
  IntLike = 1 << 1,
  // The symbol can be called (symbol(parameter))
  Callable = 1 << 2,
  // The symbol can be used as a string
  StringLike = 1 << 3,
  // The symbol is subscriptable (symbol[10])
  Subscriptable = 1 << 4,
  // The symbol is accessible (symbol.member)
  Accessible = 1 << 5,
  // The symbol is callable with new (new symbol(parameter))
  Initializable = 1 << 6
};

class SymbolTable {
public:
  std::map<intptr_t, Symbol *> symbols;
  SymbolTable();

  void write(std::ofstream &stream) const;
};

#endif
