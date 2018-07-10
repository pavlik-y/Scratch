#include "fall_detector.h"

#include "sensor_fusion.h"

void FallDetector::Setup(SensorFusion* sensor_fusion) {
  sensor_fusion_ = sensor_fusion;
  sensor_fusion_version_ = sensor_fusion_->version;
  start_upright_time_ = millis();
}

void FallDetector::Update() {
  if (sensor_fusion_version_ == sensor_fusion_->version)
    return;
  sensor_fusion_version_ = sensor_fusion_->version;
  double angle = sensor_fusion_->complementary_angle;
  if (abs(angle) > 30 & standing) {
    standing = false;
    ++version;
  }
  if (abs(angle) > 10)
    start_upright_time_ = millis();
  if ((millis() - start_upright_time_) > 1000 && !standing) {
    standing = true;
    ++version;
  }
}
