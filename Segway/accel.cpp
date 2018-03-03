#include "accel.h"

#include <math.h>

#include "command_buffer.h"
#include "config.h"
#include "gyro.h"
#include "sensor_chip.h"

Accel::Accel() = default;

void Accel::Setup(SensorChip* sensors, Gyro* gyro) {

  xBias_ = 0;
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
  xBias_ = (int)config->ReadFloat_P(kAccel_BiasX);
  zBias_ = (int)config->ReadFloat_P(kAccel_BiasZ);
}

bool Accel::HandleCommand(CommandBuffer& cb) {
  if (strcmp_P(cb.command, PSTR("RdAcc")) == 0) {
    cb.BeginResponse();
    cb.WriteValue(x);
    cb.WriteValue(z);
    cb.WriteValue(angle);
    cb.WriteValue(xBias_);
    cb.WriteValue(zBias_);
    cb.EndResponse();
    return true;
  }
  return false;
}

void Accel::ReadSample() {
  sensors_->ReadAccelData(&x, &y, &z);
  x -= xBias_;
  z -= zBias_;
  angle = atan2(double(x), double(z)) * factor_;
}
