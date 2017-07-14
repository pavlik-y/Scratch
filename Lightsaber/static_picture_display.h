#ifndef STATIC_PICTURE_DISPLAY_H_
#define STATIC_PICTURE_DISPLAY_H_

#include "motion_display.h"

class StaticPictureDisplay : public MotionDisplay {
 public:
  StaticPictureDisplay(Adafruit_NeoPixel* strip, Adafruit_LSM9DS0* sensor);

  void SetPredefinedPattern(int index);
  void SetPattern(int len, const PROGMEM byte* pattern);
  void ShowPattern(int idx, Adafruit_NeoPixel* strip) override;

 private:
  const PROGMEM byte* pattern_;
};

#endif  // STATIC_PICTURE_DISPLAY_H_