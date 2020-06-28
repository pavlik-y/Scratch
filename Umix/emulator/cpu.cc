#include "emulator/cpu.h"

#include "emulator/mem.h"
#include "emulator/instruction.h"
#include "emulator/terminal.h"

CPU::CPU(Mem* mem, Terminal* terminal)
    : mem_(mem),
      terminal_(terminal) {}

bool CPU::ExecuteSingleInstruction() {
  uint32_t instruction_code = mem_->Read(0, instruction_pointer_);
  Instruction instr = Instruction::Decode(instruction_code);
  instruction_pointer_++;
  // printf("Executing instruction: %s\n", instr.ToString().c_str());
  switch (instr.op_code) {
    case Instruction::CMOV: {
        if (regs_[instr.r_c] != 0)
          regs_[instr.r_a] = regs_[instr.r_b];
      }
      break;
    case Instruction::LD: {
        regs_[instr.r_a] = mem_->Read(regs_[instr.r_b], regs_[instr.r_c]);
      }
      break;
    case Instruction::ST: {
        mem_->Write(regs_[instr.r_a], regs_[instr.r_b], regs_[instr.r_c]);
      }
      break;
    case Instruction::ADD: {
        regs_[instr.r_a] = regs_[instr.r_b] + regs_[instr.r_c];
      }
      break;
    case Instruction::MUL: {
        regs_[instr.r_a] = regs_[instr.r_b] * regs_[instr.r_c];
      }
      break;
    case Instruction::DIV: {
        regs_[instr.r_a] = regs_[instr.r_b] / regs_[instr.r_c];
      }
      break;
    case Instruction::NOT_AND: {
        regs_[instr.r_a] = ~(regs_[instr.r_b] & regs_[instr.r_c]);
      }
      break;
    case Instruction::HALT: {
        return false;
      }
      break;
    case Instruction::ALLOC: {
        regs_[instr.r_b] = mem_->AllocPage(regs_[instr.r_c]);
      }
      break;
    case Instruction::FREE: {
        mem_->FreePage(regs_[instr.r_c]);
      }
      break;
    case Instruction::OUT: {
        terminal_->Write(regs_[instr.r_c]);
      }
      break;
    case Instruction::IN: {
        regs_[instr.r_c] = terminal_->Read();
      }
      break;
    case Instruction::LOAD_PROG: {
        if (regs_[instr.r_b] != 0)
          mem_->CopyPageToZeroPage(regs_[instr.r_b]);
        instruction_pointer_ = regs_[instr.r_c];
      }
      break;
    case Instruction::IMM: {
        regs_[instr.r_a] = instr.immed;
      }
      break;
    default:
      printf("Incorrect instruction\n");
      return false;
  }
  return true;
}