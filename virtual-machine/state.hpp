#ifndef STATE_H
#define STATE_H

#include <map>
#include <stack>
#include <tuple>
#include <vector>
#include <string>

namespace mjavac {
namespace vm {

class State {
public:
  State(std::string current_block);

  // Long to allow poth IDs (generally intptr_t) and ints to be used
  std::stack<long> stack;
  // The stack for each call made
  std::stack<std::tuple<std::string, long>> call_stack;
  // The stack for storage of the scope's state when call is made
  std::stack<std::map<std::string, int>> variable_stack;

  std::map<std::string, int> variables;
  // Instruction within the current block
  unsigned long instruction_pointer;
  std::string current_block;
};

}
}

#endif
