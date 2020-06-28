#include "emulator/instruction.h"

Instruction Instruction::Decode(uint32_t code) {
  Instruction instr;
  instr.op_code = static_cast<OP_CODE>((code >> 28) & 0xf);
  instr.immed = code & ((1 << 25) - 1);
  instr.r_a = (code >> 6) & 0x7;
  instr.r_b = (code >> 3) & 0x7;
  instr.r_c = code & 0x7;
  if (instr.op_code == IMM)
    instr.r_a = (code >> 25) & 0x7;
  return instr;
}

std::string Instruction::ToString() {
  char buffer[64];
  std::string instr_name;
  switch (op_code) {
    case CMOV:
      sprintf(buffer, "cmov r%d,r%d,r%d", r_a, r_b, r_c);
      break;
    case LD:
      sprintf(buffer, "ld r%d,r%d,r%d", r_a, r_b, r_c);
      break;
    case ST:
      sprintf(buffer, "st r%d,r%d,r%d", r_a, r_b, r_c);
      break;
    case ADD:
      sprintf(buffer, "add r%d,r%d,r%d", r_a, r_b, r_c);
      break;
    case MUL:
      sprintf(buffer, "mul r%d,r%d,r%d", r_a, r_b, r_c);
      break;
    case DIV:
      sprintf(buffer, "div r%d,r%d,r%d", r_a, r_b, r_c);
      break;
    case NOT_AND:
      sprintf(buffer, "nand r%d,r%d,r%d", r_a, r_b, r_c);
      break;
    case HALT:
      sprintf(buffer, "halt");
      break;
    case ALLOC:
      sprintf(buffer, "alloc r%d,r%d", r_b, r_c);
      break;
    case FREE:
      sprintf(buffer, "free r%d", r_c);
      break;
    case OUT:
      sprintf(buffer, "out r%d", r_c);
      break;
    case IN:
      sprintf(buffer, "inp r%d", r_c);
      break;
    case LOAD_PROG:
      sprintf(buffer, "ldprog r%d,r%d", r_b, r_c);
      break;
    case IMM:
      sprintf(buffer, "imm r%d,%d",r_a, immed);
      break;
  }

  return buffer;
}