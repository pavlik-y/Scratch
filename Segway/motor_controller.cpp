#include "motor_controller.h"

#include "config.h"
#include "fall_detector.h"
#include "motor_driver.h"
#include "tilt_controller.h"

PowerAndDirection
VelocityToPowerConverter::VelocityToPowerAndDirection(double velocity) {
  int direction = velocity < 0 ? -1 : 1;
  velocity = abs(velocity);

  PowerAndDirection result = {0.0, 0};
  if (velocity < min_velocity_)
    return result;
  // ratio = (max_power_ - min_power_) / (max_velocity_ - min_velocity_);
  double power = min_power_ +
      (velocity - min_velocity_) * ratio_;
  result.power = power;
  result.direction = direction;
  return result;
}
void VelocityToPowerConverter::SetParameters(
    double min_velocity, double max_velocity,
    double min_power, double max_power) {
  min_velocity_ = min_velocity;
  min_power_ = min_power;
  ratio_ = (max_power - min_power) / (max_velocity - min_velocity);
}

void MotorController::Setup(MotorDriver* motor_driver,
    TiltController* tilt_controller, FallDetector* fall_detector) {
  motor_driver_ = motor_driver;
  tilt_controller_ = tilt_controller;
  tilt_controller_version_ = tilt_controller_->version;
  fall_detector_ = fall_detector;
  fall_detector_version_ = fall_detector_->version;
}

void MotorController::ReadConfig(Config* config) {
  ReadConverterParameters(config, &left_converter_,
    kMotor_Left_MinVel, kMotor_Left_MaxVel,
    kMotor_Left_MinPow, kMotor_Left_MaxPow);
  ReadConverterParameters(config, &right_converter_,
    kMotor_Right_MinVel, kMotor_Right_MaxVel,
    kMotor_Right_MinPow, kMotor_Right_MaxPow);
  motor_enabled_ = config->ReadFloat_P(kMotor_Enabled) > 0.5;
  if (!motor_enabled_) {
    motor_driver_->SetPower(0, 0, 0, 0);
  }
}

void MotorController::ReadConverterParameters(
    Config* config, VelocityToPowerConverter* converter,
    const char* min_vel_name, const char* max_vel_name,
    const char* min_power_name, const char* max_power_name) {
  double min_velocity = config->ReadFloat_P(min_vel_name);
  double max_velocity = config->ReadFloat_P(max_vel_name);
  double min_power = config->ReadFloat_P(min_power_name);
  double max_power = config->ReadFloat_P(max_power_name);
  converter->SetParameters(min_velocity, max_velocity, min_power, max_power);
}

void MotorController::SetVelocity(double left_velocity, double right_velocity) {
  PowerAndDirection left = left_converter_.VelocityToPowerAndDirection(
      left_velocity);
  PowerAndDirection right = right_converter_.VelocityToPowerAndDirection(
      right_velocity);
  motor_driver_->SetPower(
      left.direction, left.power, right.direction, right.power);
  left_power = left.power * double(left.direction);
  right_power = right.power * double(right.direction);
}

void MotorController::Update() {
  // Check upstream versions (tilt_controller, fall_detector).
  // If nothing happened, return.
  if (fall_detector_version_ != fall_detector_->version) {
    if (!fall_detector_->standing)
      motor_driver_->SetPower(0, 0, 0, 0);
    fall_detector_version_ = fall_detector_->version;
  }

  if (tilt_controller_version_ == tilt_controller_->version)
    return;
  tilt_controller_version_ = tilt_controller_->version;

  if (!motor_enabled_ || !fall_detector_->standing) {
    return;
  }

  // Get target velocity.
  double target_velocity = tilt_controller_->velocity;

  // Adjust by turn offset.
  double left_velocity = target_velocity + turn_offset;
  double right_velocity = target_velocity - turn_offset;

  // Convert to PowerAndDirection
  // Pass to MotorDriver
  SetVelocity(left_velocity, right_velocity);
}

void MotorController::SetTurnOffset(double turn_offset) {
  this->turn_offset = turn_offset;
}
