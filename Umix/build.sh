CFLAGS="-O3 -std=c++11 -I ."
g++ $CFLAGS -c emulator/main.cc -o out/main.o
g++ $CFLAGS -c emulator/instruction.cc -o out/instruction.o
g++ $CFLAGS -c emulator/mem.cc -o out/mem.o
g++ $CFLAGS -c emulator/cpu.cc -o out/cpu.o
g++ -o out/emul \
  out/main.o \
  out/cpu.o \
  out/instruction.o \
  out/mem.o