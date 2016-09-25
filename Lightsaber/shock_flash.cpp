#include <Arduino.h>

#include "shock_flash.h"

#include "component_driver.h"
#include "prefs.h"

const int kSampleInterval = 20;

ShockFlash::ShockFlash(Adafruit_NeoPixel* strip, Adafruit_LSM9DS0* sensor, Prefs* prefs)
    : strip_(strip),
      sensor_(sensor),
      prefs_(prefs),
      last_sample_time_(millis()),
      sequence_running_(false),
      sequence_start_time_(0) {  
}

void ShockFlash::Stop() {
  strip_->clear();
  strip_->setBrightness(255);
  strip_->show();
}

void ShockFlash::Tick() {
  unsigned long now = millis();
  if (now - last_sample_time_ < kSampleInterval)
    return;
  last_sample_time_ = now;
  sensor_->readGyro();
  if (abs(sensor_->gyroData.y) + abs(sensor_->gyroData.z) > 40000.0) {
    sequence_start_time_ = now;
    prefs_->GetColor(&color_r_, &color_g_, &color_b_);
    sequence_running_ = true;
  }
  if (sequence_running_)
    UpdateStrip(now);
}

const int kBlinkEnd = 600;
const int kBlinkInterval = 40;
const int kDimmedEnd = kBlinkEnd + 2000;
const int kFadeDuration = 500;
const int kFadeEnd = kDimmedEnd + kFadeDuration;

const int kDimLevel = 50;

void ShockFlash::UpdateStrip(unsigned long now) {
  if (now - sequence_start_time_ < kBlinkEnd) {
    bool even_blink_interval = (((now - sequence_start_time_) / kBlinkInterval) % 2) == 0;
    strip_->setBrightness(even_blink_interval ? 255 : 0);
    SetStripColor(color_r_, color_g_, color_b_);
    return;
  }
  if (now - sequence_start_time_ < kDimmedEnd) {
    strip_->setBrightness(kDimLevel);
    SetStripColor(color_r_, color_g_, color_b_);
    return;
  }
  if (now - sequence_start_time_ < kFadeEnd) {
    unsigned long fade_progress = kFadeEnd - (now - sequence_start_time_);
    strip_->setBrightness((kDimLevel * fade_progress) / kFadeDuration);
    SetStripColor(color_r_, color_g_, color_b_);
    return;
  }
  SetStripColor(0, 0, 0);
  sequence_running_ = false;
}

void ShockFlash::SetStripColor(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < strip_->numPixels(); i++) {
    strip_->setPixelColor(i, r, g, b);
  }
  strip_->show();
}

