#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include <istream>
#include <sstream>
#include <stack>
#include <vector>

#include "bytecode.hpp"

namespace mjavac {
namespace vm {
class VirtualMachine {
public:
  mjavac::vm::Bytecode *bytecode;
  // Long to allow poth IDs (generally intptr_t) and ints to be used
  std::stack<long> stack;
  std::map<std::string, int> variables;
  size_t instruction_pointer;
  mjavac::vm::Block *current_block;

  VirtualMachine(mjavac::vm::Bytecode *bytecode);

  int step();
  int execute();
};
} // namespace vm
} // namespace mjavac

#endif
