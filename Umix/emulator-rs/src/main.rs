mod cpu;
mod instruction;
mod mem;
mod terminal;

use std::io::prelude::*;
use std::fs::File;
use std::path::Path;

struct TerminalImpl {}

impl terminal::Terminal for TerminalImpl {
  fn read(&self) -> u32 {
    std::io::stdout().flush().unwrap();
    let mut buf: [u8; 1] = [0;1];
    std::io::stdin().read(&mut buf).unwrap();
    buf[0] as u32
  }

  fn write(&self, value: u32) {
    let mut buf: [u8; 1] = [0;1];
    buf[0] = value as u8;
    std::io::stdout().write(&buf).unwrap();
    std::io::stdout().flush().unwrap();
  }
}

fn load_program(path: &Path, mem: &mut mem::Mem) {
  let mut buf: Vec<u8> = vec!();
  let mut f = File::open(path).unwrap();
  f.read_to_end(&mut buf).unwrap();
  let size = buf.len() / 4;
  mem.alloc_zero_page(size as u32);
  for i in 0..size {
    let instruction_code : u32 =
        (buf[i*4] as u32) << 24 |
        (buf[i*4 + 1] as u32) << 16 |
        (buf[i*4+2] as u32) << 8 |
        (buf[i*4+3] as u32);
    mem.write(0, i as u32, instruction_code);
  }
}

fn main() {
  let mut mem = mem::Mem::new();
  // load_program(Path::new("../Materials/sandmark.umz"), &mut mem);
  // load_program(Path::new("../Materials/codex.umz"), &mut mem);
  load_program(Path::new("../codex.um.out"), &mut mem);
  let terminal = TerminalImpl {};
  let mut cpu = cpu::CPU::new(&mut mem, &terminal);
  loop {
    if !cpu.execute_single_instruction() {
      break;
    }
  }
}
