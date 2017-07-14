#ifndef MOTION_DISPLAY_H_
#define MOTION_DISPLAY_H_

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

 protected:
  void SetPatternLen(int pattern_len);
  virtual void ShowPattern(int idx, Adafruit_NeoPixel* strip) = 0;
  virtual void OnSequenceStart();

 private:
  void AdvanceIndex();
  void RestartSequence(unsigned long now);
  void ShowFrame(int frame_idx);

  Adafruit_NeoPixel* strip_;
  Adafruit_LSM9DS0* sensor_;

  int  pattern_len_;

  int direction_;

  unsigned long sequence_start_time_;
  unsigned long sequence_interval_;
  int current_frame_;
};

#endif  // MOTION_DISPLAY_H_
