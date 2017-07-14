#ifndef RAINBOW_H_
#define RAINBOW_H_

#include <Adafruit_NeoPixel.h>

#include "component.h"

class Rainbow : public Component {
 public:
  Rainbow(Adafruit_NeoPixel* strip);

  void Start() override;
  void Stop() override;
  void Tick() override;

 private:
  Adafruit_NeoPixel* strip_;
  unsigned long last_display_time_;
  int index_;
};

#endif  // RAINBOW_H_