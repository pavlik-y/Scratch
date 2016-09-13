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

void ShockFlash::Register(ComponentDriver* component_driver) {
  component_driver->RegisterComponent(this);
}

void ShockFlash::Start() {
  
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

void ShockFlash::UpdateStrip(unsigned long now) {
  if (now - sequence_start_time_ < 200) {
    strip_->setBrightness(255);
    SetStripColor(color_r_, color_g_, color_b_);
    return;
  }
  if (now - sequence_start_time_ < 3000) {
    strip_->setBrightness(128);
    SetStripColor(color_r_, color_g_, color_b_);
    return;
  }
  if (now - sequence_start_time_ < 3500) {
    strip_->setBrightness((128*(500 - (now - sequence_start_time_ - 3000))) / 500);
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

