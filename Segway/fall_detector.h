#ifndef FALL_DETECTOR_H_
#define FALL_DETECTOR_H_

#include <Arduino.h>

#include "common.h"
#include "component.h"
#include "sensor_fusion.h"

class FallDetector : public Component{
public:
  void Setup(SensorFusion* sensor_fusion) {
    sensor_fusion_ = sensor_fusion;
    sensor_fusion_version_ = sensor_fusion_->version;
    standing = false;
    start_upright_time_ = millis();
    version = 0;
  }

  virtual void Update() {
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

  virtual bool HandleCommand(CommandBuffer& cb) {
    if (strcmp_P(cb.command, PSTR("RdFD")) == 0) {
      cb.BeginResponse();
      cb.WriteValue(standing);
      cb.WriteValue(version);
      cb.WriteValue(sensor_fusion_version_);
      cb.EndResponse();
      return true;
    }
    return false;
  }

  bool standing;
  Version version;
private:
  SensorFusion* sensor_fusion_;
  Version sensor_fusion_version_;
  unsigned long start_upright_time_;
};

#endif
