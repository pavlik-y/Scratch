#include <cstdio>

#include "emulator/cpu.h"
#include "emulator/instruction.h"
#include "emulator/mem.h"
#include "emulator/terminal.h"

class TerminalImpl : public Terminal {
 public:
  uint32_t Read() override {
    // putchar('.');
    fflush(stdout);
    return getchar();
  }

  void Write(uint32_t value) override {
    putchar(value);
  }
};

void LoadProgram(const char* fname, Mem* mem) {

  FILE* pFile = fopen(fname , "rb" );
  if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

  // obtain file size:
  fseek (pFile , 0 , SEEK_END);
  uint32_t size = ftell(pFile) / 4;
  rewind (pFile);

  mem->AllocZeroPage(size);

  for (uint32_t offset = 0; offset < size; offset ++) {
    uint8_t b[4];
    int result = fread (b, 1, 4, pFile);
    if (result != 4) {fputs ("Reading error",stderr); exit (3);}
    uint32_t value = b[0] << 24 | b[1] << 16 | b[2] << 8 | b[3];
    // uint32_t value = b[3] << 24 | b[2] << 16 | b[1] << 8 | b[0];

    mem->Write(0, offset, value);
  }
  fclose (pFile);
}

int main(void) {
  TerminalImpl terminal;
  Mem mem;
  LoadProgram("Materials/sandmark.umz", &mem);
  // LoadProgram("codex.um.out", &mem);
  CPU cpu(&mem, &terminal);
  while(true) {
    // printf(".");
    if (!cpu.ExecuteSingleInstruction())
      break;
  }
  fflush(stdout);
  return 0;
}