#ifndef VELOCITY_CONTROLLER_H_
#define VELOCITY_CONTROLLER_H_

#include <Arduino.h>

#include "common.h"
#include "component.h"

class CommandBuffer;
class Config;
class PidController;
class Position;

class VelocityController : public Component {
public:
  void Setup(PidController* velocity_to_angle, Position* position);
  void Update() override;
  bool HandleCommand(CommandBuffer& cb) override;
  void ReadConfig(Config* config) override;
  Version version;
  double angle_offset;

private:
  Position* position_;
  Version position_version_;
  PidController* velocity_to_angle_;
  unsigned long last_sample_time_;
};

#endif  // VELOCITY_CONTROLLER_H_
