#include "digital_clock.h"

DigitalClock::DigitalClock(Adafruit_NeoPixel* strip, RTC_PCF8523* rtc)
    : strip_(strip),
      rtc_(rtc),
      last_seconds_time_(0) {
}

const int kMinuteOffset = 8;
const int kHourOffset = 15;

void DigitalClock::Stop() {
  strip_->clear();
  strip_->setBrightness(255);
  strip_->show();
}

void DigitalClock::Tick() {
  DateTime now = rtc_->now();
  if (last_seconds_time_ == now.secondstime())
    return;
  last_seconds_time_ = now.secondstime();

  // Seconds
  DisplayNumber(now.second(), 0, 6, Adafruit_NeoPixel::Color(0, 0, 0), Adafruit_NeoPixel::Color(100, 0, 0)); 
  
  // Minutes
  DisplayNumber(now.minute(), kMinuteOffset, 6, Adafruit_NeoPixel::Color(0, 0, 0), Adafruit_NeoPixel::Color(0, 100, 0)); 
  
  // Hours
  DisplayNumber(now.hour(), kHourOffset, 5, Adafruit_NeoPixel::Color(0, 0, 0), Adafruit_NeoPixel::Color(0, 0, 100)); 
  strip_->show();
}

void DigitalClock::DisplayNumber(int n, int offset, int count, uint32_t c_off, uint32_t c_on) {
  for (int i = 0; i < count; i++) {
    uint32_t color = (n & 1 << i) == 0 ? c_off: c_on;
    strip_->setPixelColor(offset + i, color);
  }
}

