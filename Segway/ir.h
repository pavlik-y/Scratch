#ifndef ir_h_
#define ir_h_

#include <Arduino.h>
#include <IRremote.h>

#include "common.h"
#include "component_manager.h"


class IR : public Component {
public:
  enum Command {
    Stop,
    Forward,
    Back,
    Left,
    Right,
  };

  void Setup(int ir_pin) {
    irrecv_ = new IRrecv(ir_pin);
    irrecv_->enableIRIn();
    command = Stop;
    version = 0;
    last_command_time_ = millis();
  }

  virtual void Update() {
    unsigned long now = millis();
    if (command != Stop && (now - last_command_time_) > 250) {
      command = Stop;
      ++version;
      return;
    }

    decode_results results;
    if (!irrecv_->decode(&results))
      return;
    Serial.println(results.value, HEX);
    irrecv_->resume(); // Receive the next value
    last_command_time_ = now;

    if (results.value == 0xFFFFFFFF)
      return;
    if (command != Stop)
      return;
    if (results.value == 0xFF629D) {
      command = Forward;
      ++version;
      return;
    }
    if (results.value == 0xFF02FD) {
      command = Back;
      ++version;
      return;
    }
    if (results.value == 0xFF22DD) {
      command = Left;
      ++version;
      return;
    }
    if (results.value == 0xFFC23D) {
      command = Right;
      ++version;
      return;
    }
  }

  virtual bool HandleCommand(CommandBuffer& cb) {
    return false;
  }

  virtual void ReadConfig(Config* config) {
  }

  Version version;
  Command command;

private:
  IRrecv* irrecv_;
  unsigned long last_command_;
  unsigned long last_command_time_;
};

#endif
