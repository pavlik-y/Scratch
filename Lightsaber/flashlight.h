#ifndef FLASHLIGHT_H_
#define FLASHLIGHT_H_

#include <Adafruit_NeoPixel.h>

#include "component.h"

class Prefs;

class Flashlight : public Component {
 public:
  Flashlight(Adafruit_NeoPixel* strip, Prefs* prefs);

  void Start() override;
  void Stop() override;
  void Tick() override;

 private:
  Adafruit_NeoPixel* strip_;
  Prefs* prefs_;
  unsigned long last_display_time_;
};

#endif  // FLASHLIGHT_H_