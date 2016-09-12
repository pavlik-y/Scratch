#ifndef PREFS_H_
#define PREFS_H_

#include <Arduino.h>

class Prefs {
 public:
  Prefs();

  void SetColor(uint8_t r, uint8_t g, uint8_t b);
  void GetColor(uint8_t* r, uint8_t* g, uint8_t* b);
 private:
  uint8_t color_r_;
  uint8_t color_g_;
  uint8_t color_b_;
};

#endif  // PREFS_H_
