#include "diag.h"

#include "command_buffer.h"
#include "accel.h"
#include "gyro.h"
#include "position.h"
#include "sensor_chip.h"
#include "sensor_fusion.h"

Diag::Diag() {
  last_sample_time_micros_ = micros();
}

void Diag::Setup(Gyro* gyro, Accel* accel, SensorFusion* sensor_fusion,
    Position* position) {
  accel_ = accel;
  gyro_ = gyro;
  gyro_version_ = gyro_->version;
  sensor_fusion_ = sensor_fusion;
  position_ = position;
}

void Diag::Update() {
  if (!enabled_ || gyro_version_ == gyro_->version)
    return;
  gyro_version_ = gyro_->version;
  unsigned long now = micros();
  double dt = double(now - last_sample_time_micros_) / 1000000.0;
  last_sample_time_micros_ = now;
  gyro_angle_ += gyro_->rate * dt;

  if (sample_count++ % 5 == 0) {
    // Accel
    // Serial.printf("%f,%f,0\n", accel_->angle, gyro_angle_);

    // Gyro
    // Serial.printf("%d,%d,0\n", accel_->x, accel_->z);

    // SensorFusion
    // Serial.printf("%f,%f,0\n", accel_->angle,
    //     sensor_fusion_->complementary_angle);

    // Position
    Serial.printf("%f,0\n", position_->pos);
  }
}

bool Diag::HandleCommand(CommandBuffer& cb) {
  if (strcmp_P(cb.command, PSTR("Diag")) == 0) {
    cb.BeginResponse();
    enabled_ = cb.GetIntParam(0) != 0;
    if (enabled_) {
      gyro_angle_ = accel_->angle;
      last_sample_time_micros_ = micros();
    }
    cb.WriteValue("Ok");
    cb.EndResponse();
    return true;
  }
  return false;
}
