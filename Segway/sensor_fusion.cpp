#include "sensor_fusion.h"

#include "accel.h"
#include "config.h"
#include "gyro.h"
#include "motor.h"

void SensorFusion::Setup(Gyro* gyro, Accel* accel) {
  gyro_ = gyro;
  accel_ = accel;
  last_sample_time_micros_ = micros();
  complementary_angle = 0;
  version = 0;
  lambda_ = 0;
}

void SensorFusion::Update() {
  if (gyro_version_ != gyro_->version) {
    gyro_version_ = gyro_->version;
    UpdateAngle(micros());
    ++version;
  }
}

void SensorFusion::ReadConfig(Config* config) {
  lambda_ = config->ReadFloat_P(kSF_Lambda);
}

bool SensorFusion::HandleCommand(CommandBuffer& cb) {
  if (strcmp_P(cb.command, PSTR("RdSF")) == 0) {
    cb.BeginResponse();
    cb.WriteValue(lambda_);
    cb.WriteValue(accel_angle);
    cb.WriteValue(complementary_angle);
    cb.EndResponse();
    return true;
  }
  return false;
}

void SensorFusion::UpdateAngle(unsigned long now) {
  gyro_rate = gyro_->rate;
  accel_angle = accel_->angle;
  double dt = double(now - last_sample_time_micros_) / 1000000.0;
  last_sample_time_micros_ = now;
  complementary_angle =
      (complementary_angle + gyro_rate * dt) * (1.0 - lambda_) +
      accel_angle * lambda_;
}

