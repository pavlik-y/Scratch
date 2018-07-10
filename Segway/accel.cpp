#include "accel.h"

#include <math.h>

#include "command_buffer.h"
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
    digitalWrite(LED_BLUE, HIGH);
    ReadSample();
    digitalWrite(LED_BLUE, LOW);
    ++version;
  }
}

void Accel::ReadConfig(Config* config) {
  yBias_ = (int)config->ReadFloat_P(kAccel_BiasY);
  zBias_ = (int)config->ReadFloat_P(kAccel_BiasZ);
}

bool Accel::HandleCommand(CommandBuffer& cb) {
  if (strcmp_P(cb.command, PSTR("RdAcc")) == 0) {
    cb.BeginResponse();
    cb.WriteValue(y);
    cb.WriteValue(z);
    cb.WriteValue(angle);
    cb.WriteValue(yBias_);
    cb.WriteValue(zBias_);
    cb.EndResponse();
    return true;
  }
  return false;
}

void Accel::ReadSample() {
  int16_t x;
  sensors_->ReadAccelData(&x, &y, &z);
  y -= yBias_;
  z -= zBias_;
  angle = atan2(double(-y), double(z)) * kRadToDegFactor;
}
