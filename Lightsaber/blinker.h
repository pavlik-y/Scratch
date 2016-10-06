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
  void ShowPattern(int idx);
  
  Adafruit_NeoPixel* strip_;
  unsigned long last_update_time_;
  int sequence_idx_;
};

#endif  // BLINKER_H_
