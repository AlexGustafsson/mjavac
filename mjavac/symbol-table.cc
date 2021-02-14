#include "symbol-table.hpp"

#include <iomanip>

SymbolTable::SymbolTable() {
}

void SymbolTable::write(std::ofstream &stream) const {
  stream << "|" << std::setw(15) << "Node ID"
         << "|" << std::setw(15) << "Scope"
         << "|" << std::setw(15) << "Traits"
         << "|" << std::endl;
  stream << "|---------------|---------------|---------------|" << std::endl;
  for (const auto &symbol : this->symbols)
    stream << "|" << std::setw(15) << std::setbase(16) << symbol.first << "|" << std::setw(15) << std::setbase(16) << symbol.second->scope << "|" << std::setw(15) << symbol.second->traits << "|" << std::endl;
}
