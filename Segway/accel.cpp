#include "accel.h"

#include <math.h>

#include "config.h"
#include "gyro.h"
#include "sensor_chip.h"

Accel::Accel() = default;

void Accel::Setup(SensorChip* sensors, Gyro* gyro) {

  yBias_ = 0;
  zBias_ = 0;
  sensors_ = sensors;
  gyro_ = gyro;
  gyro_version_ = gyro_->version;
  version = 0;
}

void Accel::Update() {
  if (gyro_version_ != gyro_->version) {
    gyro_version_ = gyro_->version;
    ReadSample();
    ++version;
  }
}

void Accel::ReadConfig(Config* config) {
  yBias_ = (int)config->ReadFloat_P(kAccel_BiasY);
  zBias_ = (int)config->ReadFloat_P(kAccel_BiasZ);
}

void Accel::ReadSample() {
  int16_t x;
  sensors_->ReadAccelData(&x, &y, &z);
  y -= yBias_;
  z -= zBias_;
  angle = atan2(double(-y), double(z)) * kRadToDegFactor;
}
