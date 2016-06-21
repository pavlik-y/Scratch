#include "blinker.h"

#include "component_driver.h"

#define UPDATE_INTERVAL 1000

Blinker::Blinker(Adafruit_NeoPixel* strip)
    : strip_(strip),
      last_update_time_(0),
      sequence_idx_(0) {
  
}

void Blinker::Register(ComponentDriver* component_driver) {
  component_driver->RegisterComponent(this);
}

void Blinker::Start() {
  strip_->clear();
  strip_->show();
  last_update_time_ = millis();
}

void Blinker::Stop() {
  strip_->clear();
  strip_->show();
}

void Blinker::Tick() {
  unsigned long now = millis();
  if (now - last_update_time_ < UPDATE_INTERVAL)
    return;
  last_update_time_ = now;
  ShowPattern(sequence_idx_);  
  sequence_idx_ = (sequence_idx_ + 1) % 2;
}

void Blinker::ShowPattern(int idx) {
  for (int i = 0; i < strip_->numPixels(); i++)
    strip_->setPixelColor(i, 0, 0, idx * 128);
  strip_->show();
}


