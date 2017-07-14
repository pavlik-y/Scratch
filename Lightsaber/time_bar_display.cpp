#include "time_bar_display.h"

#include "profiler.h"

TimeBarDisplay::TimeBarDisplay(Adafruit_NeoPixel* strip, Adafruit_LSM9DS0* sensor,
    RTC_PCF8523* rtc)
    : MotionDisplay(strip, sensor),
      rtc_(rtc) {
  SetPatternLen(5);
}

void TimeBarDisplay::OnSequenceStart() {
  time_to_display_ = rtc_->now();
}

void TimeBarDisplay::ShowPattern(int idx, Adafruit_NeoPixel* strip) {
  int value = 0;
  uint32_t color = 0;
  switch (idx) {
    case 1:
      value = time_to_display_.hour() * 21 / 24;
      color = strip->Color(255, 0, 0);
      break;
    case 2:
      value = time_to_display_.minute() * 21 / 60;
      color = strip->Color(0, 255, 0);
      break;
    case 3:
      value = time_to_display_.second() * 21 / 60;
      color = strip->Color(0, 0, 255);
      break;
  }
  for (int i = 0; i < strip->numPixels(); i++)
    if (i < value)
      strip->setPixelColor(i, color);
    else
      strip->setPixelColor(i, 0);
  strip->show();
}
