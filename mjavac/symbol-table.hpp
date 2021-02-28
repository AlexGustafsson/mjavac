#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <bitset>
#include <cstdint>
#include <fstream>
#include <list>
#include <map>

#include <mjavac/ast/node.hpp>

#define SYMBOL_TRAITS 128

// TODO: Break free from the AST, using long etc. instead of intptr_t?
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
  const mjavac::ast::Node *node;
  // The (optional) name of the symbol, such as the class, method or variable name
  std::string name;
  // Symbols defined in this scope
  std::list<Symbol *> symbols;
  // An optional reference to a variable or method by name this symbol behaves the same like
  // useful for variable uses or method calls where the return value is not known.
  std::string behaves_like_identifier;
  mjavac::ast::Node *behaves_like_node;
  std::string behaves_like_object;
  Symbol *behaves_like_object_parent;

  Symbol(const mjavac::ast::Node *node, intptr_t scope, int traits)
      : scope(scope), traits(traits), node(node) {
  }
  Symbol(const mjavac::ast::Node *node, std::string name, intptr_t scope, int traits)
      : scope(scope), traits(traits), node(node), name(name) {
  }
  Symbol(const mjavac::ast::Node *node, intptr_t scope, int traits, std::string other)
      : scope(scope), traits(traits), node(node), behaves_like_identifier(other) {
  }
  Symbol(const mjavac::ast::Node *node, intptr_t scope, int traits, mjavac::ast::Node *other)
      : scope(scope), traits(traits), node(node), behaves_like_node(other) {
  }
  Symbol(const mjavac::ast::Node *node, intptr_t scope, int traits, std::string object, Symbol *parent)
      : scope(scope), traits(traits), node(node), behaves_like_object(object), behaves_like_object_parent(parent) {
  }
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
  Initializable = 1 << 6,
  // The symbol can be treated as a boolean
  BooleanLike = 1 << 7,
  // The symbol behaves like another symbol referenced in behaves_like_identifier
  BehavesLikeIdentifier = 1 << 8,
  // The symbol behaves like another symbol referenced in behaves_like_node
  BehavesLikeNode = 1 << 9,
  // The symbol behaves like another symbol which is accessed via an object
  BehavesLikeObject = 1 << 10
};

class SymbolTableView;

class SymbolTable {
public:
  std::map<intptr_t, Symbol *> symbols;
  SymbolTable();

  // Creates a view of this symbol table
  SymbolTableView *create_view() const;
  void add_symbol(Symbol *symbol);
  Symbol *get_symbol(intptr_t id) const;

  void write(std::ofstream &stream) const;
};

class SymbolTableView {
public:
  intptr_t scope;
  const SymbolTable *symbol_table;
  SymbolTableView(const SymbolTable *symbol_table);

  void set_scope(intptr_t scope);
  int count_symbols() const;
  int count_symbols_by_name(std::string name) const;
  Symbol *get_symbol_by_name(Symbol *root, std::string name) const;
  Symbol *get_symbol_by_name(std::string name) const;
  int resolve_flags(const Symbol *symbol) const;
};

#endif
