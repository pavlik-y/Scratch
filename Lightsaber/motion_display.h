#ifndef BLINKER_H_
#define BLINKER_H_

#include <Adafruit_LSM9DS0.h>
#include <Adafruit_NeoPixel.h>
#include <avr/pgmspace.h>

#include "component.h"

class MotionDisplay : public Component {
 public:
  MotionDisplay(Adafruit_NeoPixel* strip, Adafruit_LSM9DS0* sensor);
  
  void Start() override;
  void Stop() override;
  void Tick() override;

  void SetPattern(int len, const PROGMEM byte* pattern);
  void SetPredefinedPattern(int index);
  
 private: 
  void ShowPattern(int idx);
  void AdvanceIndex();
  void RestartSequence(unsigned long now);
  virtual void ShowFrame(int frame_idx);
  
  Adafruit_NeoPixel* strip_;
  Adafruit_LSM9DS0* sensor_;
  int  pattern_len_;
  const PROGMEM byte* pattern_;

  int direction_;
  
  unsigned long sequence_start_time_;
  unsigned long sequence_interval_;
  int current_frame_;
};

#endif  // BLINKER_H_
