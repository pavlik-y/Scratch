#ifndef POSITION_H_
#define POSITION_H_

#include <Arduino.h>

#include "common.h"
#include "component.h"

class Config;
class CommandBuffer;
class SwRotaryEncoder;

class Position : public Component {
public:
  void Setup(SwRotaryEncoder* right_encoder);

  void ReadConfig(Config* config) override;
  void Update() override;
  bool HandleCommand(CommandBuffer& cb) override;

  Version version;
  double pos;
  int32_t encoder_pos;
  unsigned long last_sample_time_micros;
  // double velocity;
  // unsigned long sample_time;
  // double avg_velocity;

private:
  SwRotaryEncoder* right_encoder_;
  // double last_pos_;
  // double lambda_;
  unsigned long sample_interval_micros_;
};

#endif  // POSITION_H_
