#include "gyro.h"

#include "command_buffer.h"
#include "config.h"

Gyro::Gyro()
    : device_(0x69) {}

void Gyro::Setup() {
  byte whoAmI = device_.ReadByteRegister(0xf);
  if (whoAmI != B11010011)
    halt(2000);

  // Three lowest bits of control register are enabling x,y and z
  // respectively.
  device_.WriteRegister(0x20, B00001010);

  version = 0;
}

void Gyro::ReadConfig(Config* config) {
  bias_ = config->ReadFloat_P(kGyro_Bias);
}

void Gyro::Update() {
  if (!DataAvailable())
    return;
  ReadSample();
  ++version;
}

bool Gyro::DataAvailable() {
  // Three lowest bits of status register 27 are indicating data available for
  // x,y and z respectively.
  byte status = device_.ReadByteRegister(0x27);
  return (status & B00000010) != 0;
}

// Reads rotation rate around y axis from gyro. Output is in deg/sec.
void Gyro::ReadSample() {
  short y;
  // output registers:
  // x: 28
  // y: 2A
  // z: 2C
  device_.ReadRegister(0x2a, 2, (byte*)&y);
  raw_rate_ = -y;
  rate = (double(raw_rate_) - bias_) * factor_;
  ++sample_count;
  raw_angle += (long)raw_rate_;
}

bool Gyro::HandleCommand(CommandBuffer& cb) {
  if (strcmp_P(cb.command, PSTR("RdGyro")) == 0) {
    cb.BeginResponse();
    cb.WriteValue(rate);
    cb.WriteValue(raw_angle);
    cb.WriteValue(sample_count);
    cb.WriteValue(bias_);
    cb.EndResponse();
    return true;
  }
  return false;
}
void Gyro::SetBias(double bias) {
  bias_ = bias;
}
