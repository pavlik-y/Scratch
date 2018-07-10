#include "tilt_controller.h"

#include "config.h"
#include "sensor_fusion.h"
#include "velocity_controller.h"

void TiltController::Setup(SensorFusion* sensor_fusion) {
  sensor_fusion_ = sensor_fusion;
  sensor_fusion_version_ = sensor_fusion_->version;
}

void TiltController::Update() {
  if (sensor_fusion_version_ == sensor_fusion_->version)
    return;
  UpdatePidControllers();
  sensor_fusion_version_ = sensor_fusion_->version;
  ++version;
}

void TiltController::UpdatePidControllers() {
  angle_to_velocity_.SetSetpoint(upright_angle_ + target_angle_);

  // dt is 0. Tilt controller doesn't use integral component.
  angle_to_velocity_.CalcOutput(
      sensor_fusion_->complementary_angle,
      sensor_fusion_->gyro_rate, 0.0);
  velocity = angle_to_velocity_.output;
}

void TiltController::ReadConfig(Config* config) {
  upright_angle_ = config->ReadFloat_P(kBal_UprightAngle);
  double kp = config->ReadFloat_P(kTiltCtrl_KP);
  double kd = config->ReadFloat_P(kTiltCtrl_KD);

  angle_to_velocity_.SetCoefficients(kp, 0, kd, 0);
}

void TiltController::SetTargetAngle(double angle) {
  target_angle_ = angle;
}
