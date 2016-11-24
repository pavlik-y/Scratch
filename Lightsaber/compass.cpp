#include <Arduino.h>

#include <math.h>

#include "compass.h"

const long kSampleInterval = 1000;

Compass::Compass(Adafruit_NeoPixel* strip, Adafruit_LSM9DS0* sensor)
    : strip_(strip),
      sensor_(sensor),
      last_sample_time_(0) {
}

void Compass::Start() {
  last_sample_time_ = millis();
}

void Compass::Stop() {
  strip_->clear();
  strip_->setBrightness(255);
  strip_->show();
}

void Compass::Tick() {
  unsigned long now = millis();
  if (now - last_sample_time_ < kSampleInterval)
    return;
  last_sample_time_ = now;
  sensor_->readMag();
  Serial.println("===");
  Serial.println(sensor_->magData.x);
  Serial.println(sensor_->magData.y);
  Serial.println(sensor_->magData.z);
  double angle = atan2(sensor_->magData.x, sensor_->magData.y)*180.0/M_PI;
  Serial.println(angle);
}


