pub const CMOV: u32 = 0;
pub const LD: u32 = 1;
pub const ST: u32 = 2;
pub const ADD: u32 = 3;
pub const MUL: u32 = 4;
pub const DIV: u32 = 5;
pub const NOT_AND: u32 = 6;
pub const HALT: u32 = 7;
pub const ALLOC: u32 = 8;
pub const FREE: u32 = 9;
pub const OUT: u32 = 10;
pub const IN: u32 = 11;
pub const LOAD_PROG: u32 = 12;
pub const IMM: u32 = 13;

#[derive(Debug)]
pub struct Instruction {
  pub op_code: u32,
  pub r_a: usize,
  pub r_b: usize,
  pub r_c: usize,
  pub immed: u32,
}

pub fn decode(code: u32) -> Instruction {
  let op_code = (code >> 28) & 0xf;
  let immed = code & ((1 << 25) - 1);
  let r_b = (code >> 3) & 0x7;
  let r_c = code & 0x7;
  let r_a = if op_code == IMM {
      (code >> 25) & 0x7
    } else {
      (code >> 6) & 0x7
    };
  Instruction {
    op_code,
    r_a: r_a as usize,
    r_b: r_b as usize,
    r_c: r_c as usize,
    immed,
  }
}