#include "diag.h"

#include "accel.h"
#include "command_buffer.h"
#include "config.h"
#include "gyro.h"
#include "motor_controller.h"
#include "position.h"
#include "sensor_chip.h"
#include "sensor_fusion.h"
#include "tilt_controller.h"

namespace {
unsigned long kSampleIntervalMicros = 100000;
}  // namespace

Diag::Diag() {
  last_sample_time_micros_ = micros();
}

void Diag::Setup(Gyro* gyro, Accel* accel, SensorFusion* sensor_fusion,
    Position* position,
    SwRotaryEncoder* left_encoder, SwRotaryEncoder* right_encoder,
    TiltController* tilt_controller, MotorController* motor_controller) {
  accel_ = accel;
  gyro_ = gyro;
  sensor_fusion_ = sensor_fusion;
  position_ = position;
  left_encoder_ = left_encoder;
  right_encoder_ = right_encoder;
  tilt_controller_ = tilt_controller;
  motor_controller_ = motor_controller;
}

void Diag::Update() {
  if (mode_ == Mode::DISABLED)
    return;
  unsigned long now = micros();
  unsigned long delta_micros = now - last_sample_time_micros_;
  if (delta_micros < kSampleIntervalMicros)
    return;
  last_sample_time_micros_ = now;
  // double dt = double(delta_micros) / 1000000.0;
  DumpMeasurements();

  // gyro_angle_ += gyro_->rate * dt;

  // Accel
  // Serial.printf("%f,%f,0\n", accel_->angle, gyro_angle_);

  // Gyro
  // Serial.printf("%d,%d,0\n", accel_->x, accel_->z);

  // SensorFusion
  // Serial.printf("%f,%f,0\n", accel_->angle,
  //     sensor_fusion_->complementary_angle);

  // Encoder
  // Serial.printf("%d,%d,0\n", left_encoder_->readAbs(),
  //     right_encoder_->readAbs());

  // Position
  // Serial.printf("%f,%f,%f,0\n",
  //     position_->LeftPos(), position_->velocity_right,
  //     motor_controller_->right_power);

  // MotorController
  // Serial.printf("%f,%f,%d,0\n", motor_controller_->velocity,
  //     position_->RightPos(), motor_controller_->power);
}

void Diag::DumpMeasurements() {
  switch (mode_) {
    case Mode::ACCEL:
        Serial.printf("%d,%d,%f,0\n",
            accel_->y, accel_->z, accel_->angle);
      break;
    case Mode::GYRO:
        Serial.printf("%d,%f,0\n",
            gyro_->raw_rate, gyro_->rate);
      break;
    case Mode::TILT_CONTROLLER:
        Serial.printf("%f,%f,%f,%f,0\n",
            sensor_fusion_->complementary_angle, tilt_controller_->velocity,
            motor_controller_->left_power, motor_controller_->right_power);
      break;
  }
}

void Diag::ReadConfig(Config* config) {
  mode_ = static_cast<Mode>((int)config->ReadFloat_P(kDiagMode));
}
