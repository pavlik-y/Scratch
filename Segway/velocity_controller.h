#ifndef VELOCITY_CONTROLLER_H_
#define VELOCITY_CONTROLLER_H_

#include <Arduino.h>

#include "common.h"
#include "component.h"
#include "pid_controller.h"

class Config;
class Position;
class TiltController;

class VelocityController : public Component {
public:
  void Setup(Position* position, TiltController* tilt_controller);

  void Update() override;
  void ReadConfig(Config* config) override;

  void SetTargetVelocity(double velocity);

  Version version = 0;
  double velocity = 0;
  double target_angle = 0;

private:
  Position* position_;
  Version position_version_;
  TiltController* tilt_controller_;
  PidController velocity_to_angle_;
  unsigned long last_sample_time_;
  double last_position_;
};

#endif  // VELOCITY_CONTROLLER_H_
