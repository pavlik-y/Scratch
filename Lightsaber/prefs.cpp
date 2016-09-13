#include <EEPROM.h>

#include "prefs.h"

Prefs::Prefs() {
}

void Prefs::SetColor(uint8_t r, uint8_t g, uint8_t b) {
  EEPROM.write(0, r);
  EEPROM.write(1, g);
  EEPROM.write(2, b);
}

void Prefs::GetColor(uint8_t* r, uint8_t* g, uint8_t* b) {
  *r = EEPROM.read(0); 
  *g = EEPROM.read(1); 
  *b = EEPROM.read(2); 
}

