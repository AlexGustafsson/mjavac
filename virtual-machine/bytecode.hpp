#ifndef BYTECODE_H
#define BYTECODE_H

#include <string>
#include <vector>
#include <ostream>
#include <map>

#include "instructions.hpp"

namespace mjavac {
namespace vm {

class Block {
public:
  Block(std::string identifier);
  Block(std::string identifier, std::vector<Instruction *> instructions);
  std::string identifier;
  std::vector<Instruction *> instructions;
  void write(std::ostream &stream) const;
};

class Bytecode {
public:
  std::map<std::string, Block *> blocks;
  Block * entry_point;
  Bytecode();
  Bytecode(std::map<std::string, Block *> blocks, Block *entry_point);
  void write(std::ostream &stream) const;
};

}
}

#endif
