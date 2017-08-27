#ifndef IR_H_
#define IR_H_

#include <Arduino.h>
#include <IRremote.h>

#include "common.h"
#include "component.h"


class IR : public Component {
public:
  enum Command {
    Stop,
    Forward,
    Back,
    Left,
    Right,
  };

  void Setup(int ir_pin);
  void Update() override;

  Version version;
  Command command;

private:
  IRrecv* irrecv_;
  unsigned long last_command_;
  unsigned long last_command_time_;
};

#endif  // IR_H_
