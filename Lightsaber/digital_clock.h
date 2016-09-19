#ifndef DIGITAL_CLOCK_H_
#define DIGITAL_CLOCK_H_

#include <Adafruit_NeoPixel.h>
#include <RTCLib.h>

#include "component.h"

class DigitalClock : public Component {
 public:
  DigitalClock(Adafruit_NeoPixel* strip, RTC_PCF8523* rtc);
  
  void Tick() override;
  
 private:
  void DisplayNumber(int n, int offset, int count, uint32_t c_off, uint32_t c_on);
  Adafruit_NeoPixel* strip_;
  RTC_PCF8523* rtc_;
  long last_seconds_time_;
};

#endif  // DIGITAL_CLOCK_H_
