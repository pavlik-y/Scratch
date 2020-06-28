use instruction;
use mem;
use terminal;

pub struct CPU<'a> {
  regs: [u32;8],
  instruction_pointer: u32,
  mem: &'a mut mem::Mem,
  terminal: &'a terminal::Terminal,
}

impl<'a> CPU<'a> {
  pub fn new(mem: &'a mut mem::Mem,
             terminal: &'a terminal::Terminal) -> Self {
    CPU {
      regs: [0; 8],
      instruction_pointer: 0,
      mem,
      terminal,
    }
  }

  pub fn execute_single_instruction(&mut self) -> bool {
    let instruction_code = self.mem.read(0, self.instruction_pointer);
    self.instruction_pointer += 1;
    let instr = instruction::decode(instruction_code);
    match instr.op_code {
      instruction::CMOV => {
        if self.regs[instr.r_c] != 0 {
          self.regs[instr.r_a] = self.regs[instr.r_b];
        }
      }
      instruction::LD => {
        self.regs[instr.r_a] = self.mem.read(
            self.regs[instr.r_b], self.regs[instr.r_c]);
      }
      instruction::ST => {
        self.mem.write(
            self.regs[instr.r_a], self.regs[instr.r_b], self.regs[instr.r_c]);
      }
      instruction::ADD => {
        let b: u64 = self.regs[instr.r_b] as u64;
        let c: u64 = self.regs[instr.r_c] as u64;
        self.regs[instr.r_a] = (b + c) as u32;
      }
      instruction::MUL => {
        let b: u64 = self.regs[instr.r_b] as u64;
        let c: u64 = self.regs[instr.r_c] as u64;

        self.regs[instr.r_a] = (b * c) as u32;
      }
      instruction::DIV => {
        self.regs[instr.r_a] = self.regs[instr.r_b] / self.regs[instr.r_c];
      }
      instruction::NOT_AND => {
        self.regs[instr.r_a] = !(self.regs[instr.r_b] & self.regs[instr.r_c]);
      }
      instruction::HALT => {
        return false;
      }
      instruction::ALLOC => {
        self.regs[instr.r_b] = self.mem.alloc_page(self.regs[instr.r_c]);
      }
      instruction::FREE => {
        self.mem.free_page(self.regs[instr.r_c]);
      }
      instruction::OUT => {
        self.terminal.write(self.regs[instr.r_c]);
      }
      instruction::IN => {
        self.regs[instr.r_c] = self.terminal.read();
      }
      instruction::LOAD_PROG => {
        if self.regs[instr.r_b] != 0 {
          self.mem.copy_page_to_zero_page(self.regs[instr.r_b]);
        }
        self.instruction_pointer = self.regs[instr.r_c];
      }
      instruction::IMM => {
        self.regs[instr.r_a] = instr.immed;
      }
      _ => {
        assert!(false);
      }
    }
    true
  }
}