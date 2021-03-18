#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include "bytecode.hpp"
#include "state.hpp"

namespace mjavac {
namespace vm {
class VirtualMachine {
public:
  Bytecode *bytecode;
  State *state;

  VirtualMachine(Bytecode *bytecode);

  int step();
  int execute();
};
} // namespace vm
} // namespace mjavac

#endif
