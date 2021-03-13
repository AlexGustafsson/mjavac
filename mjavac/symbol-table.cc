#include "symbol-table.hpp"

#include <iomanip>

SymbolTable::SymbolTable() {
}

SymbolTableView *SymbolTable::create_view() const {
  return new SymbolTableView(this);
}

void SymbolTable::add_symbol(Symbol *symbol) {
  this->symbols[symbol->node->get_id()] = symbol;
  if (this->symbols.count(symbol->scope) > 0)
    this->symbols[symbol->scope]->symbols.push_back(symbol);
}

Symbol *SymbolTable::get_symbol(intptr_t id) const {
  auto iterator = this->symbols.find(id);
  return iterator == this->symbols.end() ? nullptr : iterator->second;
}

void SymbolTable::write(std::ofstream &stream) const {
  stream << "| " << std::left << std::setw(15) << "ID"
         << " | " << std::left << std::setw(15) << "Name"
         << " | " << std::left << std::setw(15) << "Scope"
         << " | " << std::left << std::setw(15) << "Traits"
         << " | " << std::endl;
  stream << "|-----------------|-----------------|-----------------|-----------------|" << std::endl;
  for (const auto &symbol : this->symbols) {
    stream << "| " << std::setw(15) << std::setbase(16) << symbol.first
           << " | " << std::setw(15) << symbol.second->name
           << " | " << std::setw(15) << std::setbase(16) << symbol.second->scope
           << " | " << std::setw(14); // 14 - An extra space is always added in the cases below

    if (symbol.second->traits & SymbolTrait::None)
      stream << "None " << std::setw(0);
    if (symbol.second->traits & SymbolTrait::IntLike)
      stream << "IntLike " << std::setw(0);
    if (symbol.second->traits & SymbolTrait::Callable)
      stream << "Callable " << std::setw(0);
    if (symbol.second->traits & SymbolTrait::StringLike)
      stream << "StringLike " << std::setw(0);
    if (symbol.second->traits & SymbolTrait::Subscriptable)
      stream << "Subscriptable " << std::setw(0);
    if (symbol.second->traits & SymbolTrait::Accessible)
      stream << "Accessible " << std::setw(0);
    if (symbol.second->traits & SymbolTrait::Initializable)
      stream << "Initializable " << std::setw(0);
    if (symbol.second->traits == SymbolTrait::BooleanLike)
      stream << "BooleanLike " << std::setw(0);
    if (symbol.second->traits == SymbolTrait::BehavesLikeIdentifier)
      stream << "BehavesLikeIdentifier " << std::setw(0);
    if (symbol.second->traits == SymbolTrait::BehavesLikeNode)
      stream << "BehavesLikeNode " << std::setw(0);
    if (symbol.second->traits == SymbolTrait::BehavesLikeObject)
      stream << "BehavesLikeObject " << std::setw(0);
    stream << " |" << std::endl;
  }
}

SymbolTableView::SymbolTableView(const SymbolTable *symbol_table) {
  this->symbol_table = symbol_table;
}

void SymbolTableView::set_scope(intptr_t scope) {
  this->scope = scope;
}

int SymbolTableView::count_symbols() const {
  Symbol *root = this->symbol_table->get_symbol(this->scope);
  int count = 0;

  while (root != nullptr) {
    count = root->symbols.size();
    root = this->symbol_table->get_symbol(root->scope);
  }

  return count;
}

int SymbolTableView::count_symbols_by_name(std::string name, int traits) const {
  Symbol *root = this->symbol_table->get_symbol(this->scope);
  int count = 0;

  while (root != nullptr) {
    for (const auto &symbol : root->symbols) {
      if (symbol->name.compare(name) == 0 && symbol->traits & traits)
        count++;
    }

    root = this->symbol_table->get_symbol(root->scope);
  }

  return count;
}

int SymbolTableView::count_symbols_by_name(std::string name) const {
  return this->count_symbols_by_name(name, SymbolTrait::Any);
}

Symbol *SymbolTableView::get_symbol_by_name(Symbol *root, std::string name) const {
  while (root != nullptr) {
    for (const auto &symbol : root->symbols) {
      if (symbol->name.compare(name) == 0)
        return symbol;
    }

    root = this->symbol_table->get_symbol(root->scope);
  }

  return nullptr;
}

Symbol *SymbolTableView::get_symbol_by_name(std::string name) const {
  Symbol *root = this->symbol_table->get_symbol(this->scope);
  return this->get_symbol_by_name(root, name);
}

int SymbolTableView::resolve_flags(const Symbol *symbol) const {
  if (symbol->traits & SymbolTrait::BehavesLikeIdentifier && !symbol->behaves_like_identifier.empty()) {
    Symbol *other = this->get_symbol_by_name(symbol->behaves_like_identifier);
    if (other != nullptr)
      return other->traits;
  } else if (symbol->traits & SymbolTrait::BehavesLikeNode && symbol->behaves_like_node != nullptr) {
    Symbol *other = this->symbol_table->get_symbol(symbol->behaves_like_node->get_id());
    if (other != nullptr)
      return other->traits;
  } else if (symbol->traits & SymbolTrait::BehavesLikeObject && !symbol->behaves_like_object.empty() && symbol->behaves_like_object_parent != nullptr) {
    // Find the declared value in the scope of the parent object
    Symbol *other = this->get_symbol_by_name(symbol->behaves_like_object_parent, symbol->behaves_like_object);
    if (other != nullptr)
      return other->traits;
  }

  return symbol->traits;
}
