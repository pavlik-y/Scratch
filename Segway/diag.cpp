#include "diag.h"

#include "accel.h"
#include "config.h"
#include "fall_detector.h"
#include "gyro.h"
#include "motor_controller.h"
#include "position.h"
#include "sensor_chip.h"
#include "sensor_fusion.h"
#include "tilt_controller.h"
#include "velocity_controller.h"

namespace {
unsigned long kSampleIntervalMicros = 100000;
}  // namespace

Diag::Diag() {
  last_sample_time_micros_ = micros();
}

void Diag::Setup(Gyro* gyro, Accel* accel, SensorFusion* sensor_fusion,
    FallDetector* fall_detector, TiltController* tilt_controller,
    MotorController* motor_controller,
    SwRotaryEncoder* left_encoder, SwRotaryEncoder* right_encoder,
    Position* position, VelocityController* velocity_controller) {
  gyro_ = gyro;
  accel_ = accel;
  sensor_fusion_ = sensor_fusion;
  fall_detector_ = fall_detector;
  tilt_controller_ = tilt_controller;
  motor_controller_ = motor_controller;
  left_encoder_ = left_encoder;
  right_encoder_ = right_encoder;
  position_ = position;
  velocity_controller_ = velocity_controller;
}

void Diag::Update() {
  if (mode_ == Mode::DISABLED)
    return;
  unsigned long now = micros();
  unsigned long delta_micros = now - last_sample_time_micros_;
  if (delta_micros < kSampleIntervalMicros)
    return;
  last_sample_time_micros_ = now;
  DumpMeasurements();

  // gyro_angle_ += gyro_->rate * dt;
}

void Diag::DumpMeasurements() {
  switch (mode_) {
    case Mode::ACCEL: // 1
        Serial.printf("%d,%d,%f,0\n", accel_->y, accel_->z, accel_->angle);
      break;
    case Mode::GYRO: // 2
        Serial.printf("%d,%f,0\n", gyro_->raw_rate, gyro_->rate);
      break;
    case Mode::SENSOR_FUSION: // 3
        Serial.printf("%f,%f,%d,0\n",
            sensor_fusion_->accel_angle, sensor_fusion_->complementary_angle, fall_detector_->standing ? 30 : -30);
      break;
    case Mode::TILT_CONTROLLER: // 4
        Serial.printf("%f,%f,%f,0\n",
            sensor_fusion_->complementary_angle,
            velocity_controller_->target_angle,
            tilt_controller_->velocity);
      break;
    case Mode::MOTOR_CONTROLLER: // 5
        Serial.printf("%f,%f,%f,%f,0\n",
            tilt_controller_->velocity,
            velocity_controller_->velocity,
            motor_controller_->left_power * 100.0,
            motor_controller_->right_power * 100.0);
      break;
    case Mode::POSITION: // 6
        Serial.printf("%f,%f,0\n", position_->left_pos, position_->right_pos);
      break;
    case Mode::VELOCITY_CONTROLLER: // 7
        Serial.printf("%f,%f,0\n",
            velocity_controller_->velocity, velocity_controller_->target_angle);
      break;
  }
}

void Diag::ReadConfig(Config* config) {
  mode_ = static_cast<Mode>((int)config->ReadFloat_P(kDiagMode));
}
