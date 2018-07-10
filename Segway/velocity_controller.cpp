#include "velocity_controller.h"

#include "config.h"
#include "position.h"
#include "tilt_controller.h"

void VelocityController::Setup(
    Position* position, TiltController* tilt_controller) {
  tilt_controller_ = tilt_controller;
  position_ = position;
  position_version_ = position_->version;
  last_sample_time_ = position_->sample_time_micros;
  last_position_ = (position_->left_pos + position_->right_pos) / 2.0;
  SetTargetVelocity(0);
}

void VelocityController::Update() {
  if (position_version_ == position_->version)
    return;
  position_version_ = position_->version;
  unsigned long now = position_->sample_time_micros;

  double current_position = (position_->left_pos + position_->right_pos) / 2.0;
  double time_delta = double(now - last_sample_time_) / 1000000.0;
  velocity = (current_position - last_position_) / time_delta;
  velocity_to_angle_.CalcOutput(velocity, 0, 0);
  target_angle = velocity_to_angle_.output;
  tilt_controller_->SetTargetAngle(target_angle);

  last_sample_time_ = now;
  last_position_ = current_position;
  ++version;
}

void VelocityController::ReadConfig(Config* config) {
  double kp = config->ReadFloat_P(kVelCtrl_KP);
  double ki = config->ReadFloat_P(kVelCtrl_KI);
  double kd = config->ReadFloat_P(kVelCtrl_KD);
  double lambda = config->ReadFloat_P(kVelCtrl_KL);

  velocity_to_angle_.SetCoefficients(kp, ki, kd, lambda);
}

void VelocityController::SetTargetVelocity(double velocity) {
  velocity_to_angle_.SetSetpoint(velocity);
}
