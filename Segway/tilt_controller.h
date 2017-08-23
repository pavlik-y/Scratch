#ifndef TILT_CONTROLLER_H_
#define TILT_CONTROLLER_H_

#include <Arduino.h>

#include "common.h"
#include "component.h"

class Config;
class CommandBuffer;
class PidController;
class SensorFusion;
class VelocityController;

class TiltController : public Component {
public:
  void Setup(
      SensorFusion* sensor_fusion, PidController* angle_to_power,
      VelocityController* velocity_controller);
  void UpdatePidControllers();
  void Update() override;
  void ReadConfig(Config* config) override;
  bool HandleCommand(CommandBuffer& cb) override;

  double power;
  Version version;

private:
  SensorFusion* sensor_fusion_;
  Version sensor_fusion_version_;
  VelocityController* velocity_controller_;
  Version velocity_controller_version_;
  PidController* angle_to_power_;
  unsigned long last_micros_;
  double upright_angle_;
};

#endif  // TILT_CONTROLLER_H_
