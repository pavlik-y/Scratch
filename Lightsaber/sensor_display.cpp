#include <Arduino.h>

#include <math.h>

#include "sensor_display.h"

const long kSampleInterval = 70;

SensorDisplay::SensorDisplay(Adafruit_NeoPixel* strip, Adafruit_LSM9DS0* sensor)
    : strip_(strip),
      sensor_(sensor),
      last_sample_time_(0),
      sensor_type_(ACCELEROMETER) {
}

void SensorDisplay::Start() {
  last_sample_time_ = millis();
}

void SensorDisplay::Stop() {
  strip_->clear();
  strip_->setBrightness(255);
  strip_->show();
}

void SensorDisplay::Tick() {
  unsigned long now = millis();
  if (now - last_sample_time_ < kSampleInterval)
    return;
  last_sample_time_ = now;
  int x=0, y=0, z=0;
  if (sensor_type_ == COMPASS)
    ReadCompass(&x, &y, &z);
  else if (sensor_type_ == ACCELEROMETER)
    ReadAccelerometer(&x, &y, &z);
  else if (sensor_type_ == GYROSCOPE)
    ReadGyroscope(&x, &y, &z);

  strip_->clear();
  strip_->setPixelColor(x, strip_->getPixelColor(x) | strip_->Color(225, 0, 0));
  strip_->setPixelColor(y, strip_->getPixelColor(y) | strip_->Color(0, 255, 0));
  strip_->setPixelColor(z, strip_->getPixelColor(z) | strip_->Color(0, 0, 255));
  strip_->show();
}

void SensorDisplay::SetSensorType(SensorType sensor_type) {
  sensor_type_ = sensor_type;
}

void SensorDisplay::ReadCompass(int* x, int* y, int* z) {
  sensor_->readMag();
  float xf = sensor_->magData.x / 400.0;
  float yf = sensor_->magData.y / 400.0;
  float zf = sensor_->magData.z / 400.0;
  xf = constrain(xf, -10.0, 10.0);
  yf = constrain(yf, -10.0, 10.0);
  zf = constrain(zf, -10.0, 10.0);
  *x = int(xf) + 10;
  *y = int(yf) + 10;
  *z = int(zf) + 10;
}

void SensorDisplay::ReadAccelerometer(int* x, int* y, int* z) {
  sensor_->readAccel();
  float xf = sensor_->accelData.x/1000.0;
  float yf = sensor_->accelData.y/1000.0;
  float zf = sensor_->accelData.z/1000.0;
  xf = constrain(xf, -10.0, 10.0);
  yf = constrain(yf, -10.0, 10.0);
  zf = constrain(zf, -10.0, 10.0);
  *x = int(xf) + 10;
  *y = int(yf) + 10;
  *z = int(zf) + 10;
}

void SensorDisplay::ReadGyroscope(int* x, int* y, int* z) {
  sensor_->readGyro();
  float xf = sensor_->gyroData.x/1000.0;
  float yf = sensor_->gyroData.y/1000.0;
  float zf = sensor_->gyroData.z/1000.0;
  Serial.print(xf);
  Serial.print(" ");
  Serial.print(yf);
  Serial.print(" ");
  Serial.print(zf);
  Serial.println();
}
