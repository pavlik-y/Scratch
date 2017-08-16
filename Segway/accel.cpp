#include "accel.h"

#include <math.h>

#include "command_buffer.h"
#include "config.h"
#include "gyro.h"

Accel::Accel()
    : device_(0x53) {}

void Accel::Setup(Gyro* gyro) {
  byte devId = device_.ReadByteRegister(0x0);
  if (devId != B11100101)
    halt(1000);
  device_.WriteRegister(0x2D, B00001000);
  device_.WriteRegister(0x31, B00001011);

  xBias_ = 0;
  zBias_ = 0;
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
  // Registers:
  //   32: x
  //   34: y
  //   36: z
  device_.ReadRegister(0x32, 2, (byte*)&x);
  device_.ReadRegister(0x36, 2, (byte*)&z);
  x -= xBias_;
  z -= zBias_;
  angle = atan2(double(x), double(z)) * factor_;
}
