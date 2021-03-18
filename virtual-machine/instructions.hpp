#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <map>
#include <ostream>
#include <stack>
#include <string>

#include "state.hpp"

namespace mjavac {
namespace vm {

class Instruction {
public:
  virtual void perform(State *state) const = 0;
  virtual void write(std::ostream &stream) const = 0;
};

class Instruction_iload : public Instruction {
public:
  std::string identifier;
  Instruction_iload(std::string identifier);
  void perform(State *state) const;
  void write(std::ostream &stream) const;
};

class Instruction_iconst : public Instruction {
public:
  long value;
  Instruction_iconst(long value);
  void perform(State *state) const;
  void write(std::ostream &stream) const;
};

class Instruction_istore : public Instruction {
public:
  std::string identifier;
  Instruction_istore(std::string identifier);
  void perform(State *state) const;
  void write(std::ostream &stream) const;
};

class Instruction_iadd : public Instruction {
public:
  Instruction_iadd();
  void perform(State *state) const;
  void write(std::ostream &stream) const;
};

class Instruction_isub : public Instruction {
public:
  Instruction_isub();
  void perform(State *state) const;
  void write(std::ostream &stream) const;
};

class Instruction_imul : public Instruction {
public:
  Instruction_imul();
  void perform(State *state) const;
  void write(std::ostream &stream) const;
};

class Instruction_idiv : public Instruction {
public:
  Instruction_idiv();
  void perform(State *state) const;
  void write(std::ostream &stream) const;
};

class Instruction_ilt : public Instruction {
public:
  Instruction_ilt();
  void perform(State *state) const;
  void write(std::ostream &stream) const;
};

class Instruction_iand : public Instruction {
public:
  Instruction_iand();
  void perform(State *state) const;
  void write(std::ostream &stream) const;
};

class Instruction_ior : public Instruction {
public:
  Instruction_ior();
  void perform(State *state) const;
  void write(std::ostream &stream) const;
};

class Instruction_inot : public Instruction {
public:
  Instruction_inot();
  void perform(State *state) const;
  void write(std::ostream &stream) const;
};

class Instruction_goto : public Instruction {
public:
  long target;
  Instruction_goto(long target);
  void perform(State *state) const;
  void write(std::ostream &stream) const;
};

class Instruction_iffalse : public Instruction {
public:
  long target;
  Instruction_iffalse(long target);
  void perform(State *state) const;
  void write(std::ostream &stream) const;
};

class Instruction_invokevirtual : public Instruction {
public:
  std::string identifier;
  Instruction_invokevirtual(std::string identifier);
  void perform(State *state) const;
  void write(std::ostream &stream) const;
};

class Instruction_ireturn : public Instruction {
public:
  Instruction_ireturn();
  void perform(State *state) const;
  void write(std::ostream &stream) const;
};

class Instruction_print : public Instruction {
public:
  Instruction_print();
  void perform(State *state) const;
  void write(std::ostream &stream) const;
};

class Instruction_stop : public Instruction {
public:
  Instruction_stop();
  void perform(State *state) const;
  void write(std::ostream &stream) const;
};

} // namespace vm
} // namespace mjavac

#endif
