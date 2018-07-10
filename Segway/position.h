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
  void Setup(SwRotaryEncoder* left_encoder, SwRotaryEncoder* right_encoder);

  void ReadConfig(Config* config) override;
  void Update() override;

  unsigned long sample_time_micros;
  Version version;
  double left_pos;
  double right_pos;

private:
  SwRotaryEncoder* left_encoder_;
  SwRotaryEncoder* right_encoder_;
  unsigned long sample_interval_micros_;
};

#endif  // POSITION_H_
