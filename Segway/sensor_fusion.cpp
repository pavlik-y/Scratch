#include "sensor_fusion.h"

#include "accel.h"
#include "config.h"
#include "gyro.h"

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

void SensorFusion::UpdateAngle(unsigned long now) {
  gyro_rate = gyro_->rate;
  accel_angle = accel_->angle;
  double dt = double(now - last_sample_time_micros_) / 1000000.0;
  last_sample_time_micros_ = now;
  complementary_angle =
      (complementary_angle + gyro_rate * dt) * (1.0 - lambda_) +
      accel_angle * lambda_;
}

