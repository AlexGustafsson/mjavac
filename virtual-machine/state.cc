#include "state.hpp"

using namespace mjavac::vm;

State::State(std::string current_block)
    : instruction_pointer(0), current_block(current_block) {
}
