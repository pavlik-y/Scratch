#ifndef CPU_H_
#define CPU_H_

#include <cstdint>

class Instruction;
class Mem;
class Terminal;

class CPU {
 public:
  CPU(Mem* mem, Terminal* terminal);

  bool ExecuteSingleInstruction();

 private:
  uint32_t regs_[8] = {0};
  uint32_t instruction_pointer_ = 0;
  Mem* mem_;
  Terminal* terminal_;
};

#endif  // CPU_H_