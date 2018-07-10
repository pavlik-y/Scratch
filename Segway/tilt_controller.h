#ifndef TILT_CONTROLLER_H_
#define TILT_CONTROLLER_H_

#include <Arduino.h>

#include "common.h"
#include "component.h"
#include "pid_controller.h"

class Config;
class PidController;
class SensorFusion;

class TiltController : public Component {
public:
  void Setup(SensorFusion* sensor_fusion);

  void Update() override;
  void ReadConfig(Config* config) override;

  void SetTargetAngle(double angle);

  double velocity = 0;
  Version version = 0;

private:
  void UpdatePidControllers();

  SensorFusion* sensor_fusion_;
  Version sensor_fusion_version_;

  double upright_angle_ = 0;
  double target_angle_ = 0;
  PidController angle_to_velocity_;
};

#endif  // TILT_CONTROLLER_H_
