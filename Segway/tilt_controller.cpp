#include "tilt_controller.h"

#include "config.h"
#include "sensor_fusion.h"
#include "velocity_controller.h"

void TiltController::Setup(SensorFusion* sensor_fusion,
                           VelocityController* velocity_controller) {
  sensor_fusion_ = sensor_fusion;
  sensor_fusion_version_ = sensor_fusion_->version;
  velocity_controller_ = velocity_controller;
  velocity_controller_version_ = velocity_controller_->version;
  last_micros_ = micros();
  power = 0;
  version = 0;
}

void TiltController::UpdatePidControllers() {
  unsigned long now_micros = micros();
  double offset = velocity_controller_->angle_offset;
  angle_to_power_.SetSetpoint(upright_angle_ + offset);

  // dt is 0. Tilt controller doesn't use integral component.
  angle_to_power_.CalcOutput(
      sensor_fusion_->complementary_angle,
      sensor_fusion_->gyro_rate, 0.0);
  last_micros_ = now_micros;
  power = constrain(angle_to_power_.output, -1.0, 1.0);
}

void TiltController::Update() {
  if (sensor_fusion_version_ == sensor_fusion_->version &&
      velocity_controller_version_ == velocity_controller_->version)
    return;
  UpdatePidControllers();
  sensor_fusion_version_ = sensor_fusion_->version;
  velocity_controller_version_ = velocity_controller_->version;
  ++version;
}

void TiltController::ReadConfig(Config* config) {
  upright_angle_ = config->ReadFloat_P(kBal_UprightAngle);
  double kp = config->ReadFloat_P(kTiltCtrl_KP);
  double ki = config->ReadFloat_P(kTiltCtrl_KI);
  double kd = config->ReadFloat_P(kTiltCtrl_KD);
  double lambda = config->ReadFloat_P(kTiltCtrl_KL);

  angle_to_power_.SetSetpoint(upright_angle_);
  angle_to_power_.SetCoefficients(kp, ki, kd, lambda);
}

bool TiltController::HandleCommand(CommandBuffer& cb) {
  if (strcmp_P(cb.command, PSTR("RdTilt")) == 0) {
    cb.BeginResponse();
    cb.WriteValue(power);
    cb.WriteValue(angle_to_power_.ie);
    cb.WriteValue(version);
    cb.WriteValue(angle_to_power_.setpoint);
    cb.EndResponse();
    return true;
  }
  return false;
}
