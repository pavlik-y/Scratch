#ifndef TIME_BAR_DISPLAY_H_
#define TIME_BAR_DISPLAY_H_

#include <RTClib.h>

#include "motion_display.h"

class TimeBarDisplay : public MotionDisplay {
 public:
  TimeBarDisplay(Adafruit_NeoPixel* strip, Adafruit_LSM9DS0* sensor,
      RTC_PCF8523* rtc);

  void ShowPattern(int idx, Adafruit_NeoPixel* strip) override;
  void OnSequenceStart() override;

 private:
  RTC_PCF8523* rtc_;
  DateTime time_to_display_;
};

#endif  // TIME_BAR_DISPLAY_H_