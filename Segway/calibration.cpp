#include "calibration.h"

#include "accel.h"
#include "gyro.h"
#include "motor.h"
#include "sensor_fusion.h"

void Calibration::Setup(Accel* accel, Gyro* gyro, SensorFusion* sensor_fusion, MotorDriver* motor_driver) {
  state = 2;
  accel_ = accel;
  gyro_ = gyro;
  gyro_version_ = gyro_->version;
  sensor_fusion_ = sensor_fusion;
  motor_driver_ = motor_driver;

  avg_accel_angle = 0;
  accel_angle_div = 0;
  avg_lambda = 0.01;
  calibration_duration = 10000;
  version = 0;
}

void Calibration::Update() {
  if (gyro_version_ == gyro_->version)
    return;
  gyro_version_ = gyro_->version;
  avg_accel_angle = LowPassFilter(avg_accel_angle, accel_->angle, avg_lambda);
  accel_angle_div = LowPassFilter(accel_angle_div, abs(accel_->angle - avg_accel_angle), avg_lambda);
  switch (state) {
    case 0: {
      if (accel_angle_div >= 0.01)
        break;
      if (abs(avg_accel_angle) < 60)
        break;
      calibration_start_time = millis();
      gyro_angle = gyro_->raw_angle;
      gyro_sample_count = gyro_->sample_count;
      state = 1;
      break;
    }
    case 1: {
      if (accel_angle_div >= 0.01) {
        state = 0;
        break;
      }
      unsigned long duration = millis() - calibration_start_time;
      if (duration < calibration_duration)
        break;
      gyro_bias = double(gyro_->raw_angle - gyro_angle) / double(gyro_->sample_count - gyro_sample_count);
      gyro_->SetBias(gyro_bias);
      sensor_fusion_->SynchronizeAngles(avg_accel_angle);
      state = 2;
      ++version;
      break;
    }
    case 2: {
      break;
    }
  }
}

bool Calibration::HandleCommand(CommandBuffer& cb) {
  if (strcmp_P(cb.command, PSTR("RdCal")) == 0) {
    cb.BeginResponse();
    cb.WriteValue(state);
    cb.WriteValue(avg_accel_angle);
    cb.WriteValue(accel_angle_div);
    cb.WriteValue(gyro_bias);
    cb.EndResponse();
    return true;
  }
  if (strcmp_P(cb.command, PSTR("Calibrate")) == 0) {
    state = 0;
    cb.BeginResponse();
    cb.WriteValue(state);
    cb.EndResponse();
    return true;
  }
  return false;
}
