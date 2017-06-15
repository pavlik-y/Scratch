#ifndef BLINKER_H_
#define BLINKER_H_

#include <Adafruit_LSM9DS0.h>
#include <Adafruit_NeoPixel.h>
#include <avr/pgmspace.h>

#include "component.h"

class Blinker : public Component {
 public:
  Blinker(Adafruit_NeoPixel* strip, Adafruit_LSM9DS0* sensor);
  
  void Start() override;
  void Stop() override;
  void Tick() override;

  void SetPattern(unsigned long interval, int len, const PROGMEM byte* pattern, bool wrap_around);
  void SetPredefinedPattern(int index);
  
 private: 
  void ShowPattern(int idx);
  void AdvanceIndex();
  
  Adafruit_NeoPixel* strip_;
  Adafruit_LSM9DS0* sensor_;
  unsigned long interval_;
  int  pattern_len_;
  const PROGMEM byte* pattern_;
  bool wrap_around_;
  int direction_;
  
  unsigned long last_update_time_;
  int sequence_idx_;
};

#endif  // BLINKER_H_
