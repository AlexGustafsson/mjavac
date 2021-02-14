#include "symbol-table.hpp"

#include <iomanip>

SymbolTable::SymbolTable() {
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
