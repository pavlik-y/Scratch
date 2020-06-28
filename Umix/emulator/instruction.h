#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <cstdint>
#include <string>

class Instruction {
 public:
  enum OP_CODE {
    CMOV = 0,
    LD = 1,
    ST = 2,
    ADD = 3,
    MUL = 4,
    DIV = 5,
    NOT_AND = 6,
    HALT = 7,
    ALLOC = 8,
    FREE = 9,
    OUT = 10,
    IN = 11,
    LOAD_PROG = 12,
    IMM = 13
  };

  static Instruction Decode(uint32_t code);
  std::string ToString();

  OP_CODE op_code;
  int r_a;
  int r_b;
  int r_c;
  uint32_t immed;
};

#endif  // INSTRUCTION_H_