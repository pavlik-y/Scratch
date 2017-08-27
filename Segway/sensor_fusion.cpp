#include "sensor_fusion.h"

#include "accel.h"
#include "config.h"
#include "gyro.h"
#include "motor.h"

void SensorFusion::Setup(Gyro* gyro, Accel* accel) {
  gyro_ = gyro;
  accel_ = accel;
  last_sample_time_ = millis();
  gyro_angle = 0;
  avg_accel_angle = 0;
  complementary_angle = 0;
  version = 0;
  lambda_ = 0;
}

void SensorFusion::Update() {
  if (gyro_version_ != gyro_->version) {
    gyro_version_ = gyro_->version;
    UpdateAngle();
    ++version;
  }
}

void SensorFusion::ReadConfig(Config* config) {
  lambda_ = config->ReadFloat_P(kSF_Lambda);
}

void SensorFusion::UpdateAngle() {
  gyro_rate = gyro_->rate;
  accel_angle = accel_->angle;
  avg_accel_angle = LowPassFilter(avg_accel_angle, accel_angle, 0.1);
  double dt = double(millis() - last_sample_time_) / 1000.0;
  if (abs(gyro_angle - avg_accel_angle) > 90)
    gyro_angle = accel_angle;
  gyro_angle += gyro_rate * dt;
  complementary_angle += (accel_angle - complementary_angle) * lambda_ + gyro_rate * dt * (1.0 - lambda_);
  last_sample_time_ = millis();
}

bool SensorFusion::HandleCommand(CommandBuffer& cb) {
  if (strcmp_P(cb.command, PSTR("RdSF")) == 0) {
    cb.BeginResponse();
    cb.WriteValue(gyro_angle);
    cb.WriteValue(accel_angle);
    cb.WriteValue(complementary_angle);
    cb.EndResponse();
    return true;
  }
  return false;
}

void SensorFusion::SynchronizeAngles(double angle) {
  gyro_angle = angle;
  complementary_angle = angle;
}
