#include "blinker.h"

#define UPDATE_INTERVAL 20

Blinker::Blinker(Adafruit_NeoPixel* strip)
    : strip_(strip),
      last_update_time_(0),
      sequence_idx_(0) {
  
}

void Blinker::Start() {
  last_update_time_ = millis();
}

void Blinker::Stop() {
  strip_->clear();
  strip_->setBrightness(255);
  strip_->show();
}

void Blinker::Tick() {
  unsigned long now = millis();
  if (now - last_update_time_ < UPDATE_INTERVAL)
    return;
  last_update_time_ = now;
  ShowPattern(sequence_idx_);  
  sequence_idx_ = (sequence_idx_ + 1) % 3;
}

void Blinker::ShowPattern(int idx) {
  int level = 128;
  int r = (idx % 3) == 0 ? level : 0;
  int g = (idx % 3) == 1 ? level : 0;
  int b = (idx % 3) == 2 ? level : 0;
  for (int i = 0; i < strip_->numPixels(); i++)
    strip_->setPixelColor(i, r, g, b);
  strip_->show();
}


