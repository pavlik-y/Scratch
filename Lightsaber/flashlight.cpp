#include "flashlight.h"

#include "prefs.h"

const long kDisplayInterval = 100;

Flashlight::Flashlight(Adafruit_NeoPixel* strip, Prefs* prefs)
    : strip_(strip),
      prefs_(prefs) {
}

void Flashlight::Start() {
  last_display_time_ = millis();
}

void Flashlight::Stop() {
  strip_->clear();
  strip_->setBrightness(255);
  strip_->show();
}

void Flashlight::Tick() {
  unsigned long now = millis();
  if (now - last_display_time_ < kDisplayInterval)
    return;
  last_display_time_ = now;

  uint8_t r;
  uint8_t g;
  uint8_t b;
  prefs_->GetColor(&r, &g, &b);

  for (int i = 0; i < strip_->numPixels(); i++) {
    strip_->setPixelColor(i, r, g, b);
  }
  strip_->show();
}