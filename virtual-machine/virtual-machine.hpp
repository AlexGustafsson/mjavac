#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include <stack>
#include <istream>
#include <sstream>
#include <vector>

#include "bytecode.hpp"

namespace mjavac {
namespace vm {
class VirtualMachine {
public:
  const mjavac::vm::Bytecode *bytecode;
  // Long to allow poth IDs (generally intptr_t) and ints to be used
  std::stack<long> stack;
  std::map<std::string, int> variables;
  size_t instruction_pointer;
  const mjavac::vm::Block *current_block;

  VirtualMachine(mjavac::vm::Bytecode *bytecode);

  int step();
  int execute();
};
}
}

#endif
