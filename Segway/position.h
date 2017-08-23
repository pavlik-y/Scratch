#ifndef POSITION_H_
#define POSITION_H_

#include <Arduino.h>

#include "common.h"
#include "component.h"

class Config;
class CommandBuffer;
class MotorEncoder;

class Position : public Component {
public:
  void Setup(MotorEncoder* right_encoder);
  void ReadConfig(Config* config) override;
  void Update() override;
  bool HandleCommand(CommandBuffer& cb) override;

  Version version;
  double pos;
  double velocity;
  unsigned long sample_time;
  double avg_velocity;

private:
  MotorEncoder* right_encoder_;
  double last_pos_;
  double lambda_;
  double sample_interval_;
  bool dump_;
};

#endif  // POSITION_H_
