#include "prefs.h"

Prefs::Prefs()
    : color_r_(0),
      color_g_(0),
      color_b_(0) {
}

void Prefs::SetColor(uint8_t r, uint8_t g, uint8_t b) {
  color_r_ = r;
  color_g_ = g;
  color_b_ = b;
}

void Prefs::GetColor(uint8_t* r, uint8_t* g, uint8_t* b) {
  *r = color_r_; 
  *g = color_g_; 
  *b = color_b_; 
}

