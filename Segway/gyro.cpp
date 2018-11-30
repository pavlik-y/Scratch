#include "gyro.h"

#include "config.h"
#include "sensor_chip.h"

Gyro::Gyro() = default;

void Gyro::Setup(SensorChip* sensors) {
  sensors_ = sensors;
  version = 0;
}

void Gyro::ReadConfig(Config* config) {
  bias_ = config->ReadFloat_P(kGyro_Bias);
}

void Gyro::Update() {
  if (!sensors_->GyroDataAvailable())
    return;
  ReadSample();
  ++version;
}

// Reads rotation rate around x axis from gyro. Output is in deg/sec.
void Gyro::ReadSample() {
  int16_t x;
  int16_t y;
  int16_t z;
  sensors_->ReadGyroData(&x, &y, &z);
  raw_rate = x;
  rate = (double(raw_rate) - bias_) * sensors_->kGyroFactor;
}
