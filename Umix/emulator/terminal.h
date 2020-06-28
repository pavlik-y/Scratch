#ifndef TERMINAL_H_
#define TERMINAL_H_

#include <cstdint>

class Terminal {
 public:
  virtual uint32_t Read() = 0;
  virtual void Write(uint32_t value) = 0;
};

#endif  // TERMINAL_H_