#include "blinker.h"

const long kDisplayInterval = 10;
Blinker::Blinker(Adafruit_NeoPixel* strip)
    : strip_(strip) {
}

void Blinker::Start() {
  last_display_time_ = millis();
  index_ = 0;
}

void Blinker::Stop() {
  strip_->clear();
  strip_->setBrightness(255);
  strip_->show();
}

void Blinker::Tick() {
  unsigned long now = millis();
  if (now - last_display_time_ < kDisplayInterval)
    return;
  last_display_time_ = now;
  for (int i = 0; i < strip_->numPixels(); i++) {
    int r = (index_ == 0) ? 255 : 0;
    int g = (index_ == 1) ? 255 : 0;
    int b = (index_ == 2) ? 255 : 0;
    strip_->setPixelColor(i, r, g, b);
  }
  index_ = (index_ + 1) % 3;
  strip_->show();
}