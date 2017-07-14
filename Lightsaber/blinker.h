#ifndef BLINKER_H_
#define BLINKER_H_

#include <Adafruit_NeoPixel.h>

#include "component.h"

class Blinker : public Component {
 public:
  Blinker(Adafruit_NeoPixel* strip);

  void Start() override;
  void Stop() override;
  void Tick() override;

 private:
  Adafruit_NeoPixel* strip_;
  unsigned long last_display_time_;
  int index_;
};

#endif  // BLINKER_H_