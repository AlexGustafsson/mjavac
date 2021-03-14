#include "bytecode.hpp"

using namespace mjavac::vm;

Block::Block(std::string identifier)
  : identifier(identifier) {
  }

Block::Block(std::string identifier, std::vector<Instruction *> instructions)
  : Block(identifier) {
  this->instructions = instructions;
}

void Block::write(std::ostream &stream) const {
  stream << this->identifier << ":" << std::endl;
  for (auto instruction : this->instructions)
    instruction->write(stream);
}

Bytecode::Bytecode() {

}

Bytecode::Bytecode(std::map<std::string, Block *> blocks, Block *entry_point) :
  blocks(blocks), entry_point(entry_point) {
}

void Bytecode::write(std::ostream &stream) const {
  for (auto &[_, block] : this->blocks) {
    block->write(stream);
    stream << std::endl;
  }
}
