#ifndef PREFS_H_
#define PREFS_H_

#include <Arduino.h>

class Prefs {
 public:
  Prefs();

  void SetColor(uint8_t r, uint8_t g, uint8_t b);
  void GetColor(uint8_t* r, uint8_t* g, uint8_t* b);

 private:
};

#endif  // PREFS_H_
