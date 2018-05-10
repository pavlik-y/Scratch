#ifndef FALL_DETECTOR_H_
#define FALL_DETECTOR_H_

#include <Arduino.h>

#include "common.h"
#include "component.h"

class SensorFusion;

class FallDetector : public Component{
public:
  void Setup(SensorFusion* sensor_fusion);

  void Update() override;
  bool HandleCommand(CommandBuffer& cb) override;

  bool standing = false;
  Version version = 0;

private:
  SensorFusion* sensor_fusion_;
  Version sensor_fusion_version_;
  unsigned long start_upright_time_;
};

#endif  // FALL_DETECTOR_H_
