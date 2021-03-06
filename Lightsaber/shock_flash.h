#ifndef SHOCK_FLASH_H_
#define SHOCK_FLASH_H_

#include <Adafruit_LSM9DS0.h>
#include <Adafruit_NeoPixel.h>

#include "component.h"
#include "effect_sequence.h"

class Prefs;

class ShockFlash : public Component {
 public:
  ShockFlash(Adafruit_NeoPixel* strip, Adafruit_LSM9DS0* sensor, Prefs* prefs);

  void Start() override;
  void Stop() override;
  void Tick() override;
  
 private:
  void UpdateStrip(unsigned long now);
  void SetStripColor(uint8_t r, uint8_t g, uint8_t b);
  
  Adafruit_NeoPixel* strip_;
  Adafruit_LSM9DS0* sensor_;
  Prefs* prefs_;
  EffectSequence effect_sequence_;
  
  unsigned long last_sample_time_;
  uint8_t color_r_;
  uint8_t color_g_;
  uint8_t color_b_;
};
#endif  // SHOCK_FLASH_H_
