#include "gyro.h"

#include "command_buffer.h"
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

// Reads rotation rate around y axis from gyro. Output is in deg/sec.
void Gyro::ReadSample() {
  int16_t x;
  int16_t y;
  int16_t z;
  sensors_->ReadGyroData(&x, &y, &z);
  raw_rate_ = -y;
  rate = (double(raw_rate_) - bias_) * sensors_->kGyroFactor;
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
