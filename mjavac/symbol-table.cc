#include "symbol-table.hpp"

#include <iomanip>

SymbolTable::SymbolTable() {
}

SymbolTableView* SymbolTable::create_view() const {
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
           << " | " << std::setw(15) << std::setbase(16) << symbol.second->scope << " | ";

    if (symbol.second->traits & SymbolTrait::Accessible)
      stream << "Accessible ";
    if (symbol.second->traits & SymbolTrait::Callable)
      stream << "Callable ";
    if (symbol.second->traits & SymbolTrait::IntLike)
      stream << "IntLike ";
    if (symbol.second->traits & SymbolTrait::StringLike)
      stream << "StringLike ";
    if (symbol.second->traits & SymbolTrait::Subscriptable)
      stream << "Subscriptable ";
    if (symbol.second->traits & SymbolTrait::Initializable)
      stream << "Initializable ";
    if (symbol.second->traits == SymbolTrait::None)
      stream << "None ";
    stream << " |" << std::endl;
  }
}

SymbolTableView::SymbolTableView(const SymbolTable* symbol_table) {
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

int SymbolTableView::count_symbols_by_name(std::string name) const {
  Symbol *root = this->symbol_table->get_symbol(this->scope);
  int count = 0;

  while (root != nullptr) {
    for (const auto &symbol : root->symbols) {
      if (symbol->name.compare(name) == 0)
        count++;
    }

    root = this->symbol_table->get_symbol(root->scope);
  }

  return count;
}
