#include <Arduino.h>

#include "shock_flash.h"

#include "prefs.h"

const int kSampleInterval = 20;
const int kBlinkInterval = 40;

EffectSequence::Step steps[] = {
  {600, 40},
  {2000, 2000},
  {500, 20}
};

ShockFlash::ShockFlash(Adafruit_NeoPixel* strip, Adafruit_LSM9DS0* sensor, Prefs* prefs)
    : strip_(strip),
      sensor_(sensor),
      prefs_(prefs),
      effect_sequence_(steps, 3),
      last_sample_time_(millis()) {  
}

void ShockFlash::Start() {
  last_sample_time_ = millis();
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
    effect_sequence_.Start(now);
    prefs_->GetColor(&color_r_, &color_g_, &color_b_);
  }
  if (effect_sequence_.DisplayUpdateNeeded(now)) {
    UpdateStrip(now);
    effect_sequence_.DisplayUpdated(now);
  }
}

const int kDimLevel = 50;

void ShockFlash::UpdateStrip(unsigned long now) {
  size_t step_index;
  unsigned long time_offset;
  effect_sequence_.GetStep(now, &step_index, &time_offset);
  unsigned long step_duration = effect_sequence_.GetStepDuration(step_index);

  switch (step_index) {
    case 0: {
      bool even_blink_interval = ((time_offset / kBlinkInterval) % 2) == 0;
      strip_->setBrightness(even_blink_interval ? 255 : 0);
      SetStripColor(color_r_, color_g_, color_b_);
      break;
    }
    case 1: {
      strip_->setBrightness(kDimLevel);
      SetStripColor(color_r_, color_g_, color_b_);
      break;
    }
    case 2: {
      strip_->setBrightness(kDimLevel * (step_duration - time_offset) / step_duration);
      SetStripColor(color_r_, color_g_, color_b_);
      break;
    }
    default: {
      SetStripColor(0, 0, 0);
      break;
    }
  }
}

void ShockFlash::SetStripColor(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < strip_->numPixels(); i++) {
    strip_->setPixelColor(i, r, g, b);
  }
  strip_->show();
}

