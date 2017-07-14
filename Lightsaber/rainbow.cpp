#include "rainbow.h"

const long kDisplayInterval = 100;

Rainbow::Rainbow(Adafruit_NeoPixel* strip)
    : strip_(strip) {
}

void Rainbow::Start() {
  last_display_time_ = millis();
  index_ = 0;
}

void Rainbow::Stop() {
  strip_->clear();
  strip_->setBrightness(255);
  strip_->show();
}

void Rainbow::Tick() {
  unsigned long now = millis();
  if (now - last_display_time_ < kDisplayInterval)
    return;
  last_display_time_ = now;
  index_ = (index_ + 1) % 21;

  for (int i = 0; i < 7; i++) {
    strip_->setPixelColor((index_ + i) % 21,
        (7 - i) * 255 / 7, (i * 255 / 7), 0);
  }
  for (int i = 0; i < 7; i++) {
    strip_->setPixelColor((index_ + i + 7) % 21,
        0, (7 - i) * 255 / 7, (i * 255 / 7));
  }
  for (int i = 0; i < 7; i++) {
    strip_->setPixelColor((index_ + i + 14) % 21,
        (i * 255 / 7), 0, (7 - i) * 255 / 7 );
  }
  strip_->show();
}