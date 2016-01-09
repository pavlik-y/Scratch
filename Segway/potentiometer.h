#ifndef potentiometer_h_
#define potentiometer_h_

#include <Arduino.h>
#include "common.h"
#include "component.h"

class Potentiometer : public Component {
public:
  
  void Setup() {
    version = 0;
    last_sample = millis();
  }
  
  virtual void Update() {
    unsigned long now = millis();
    if ((now - last_sample) < 10) 
      return;
    value = double(analogRead(2)) / 1024.0;
    last_sample = now;
    ++version;
  }

  virtual bool HandleCommand(CommandBuffer& cb) {
    if (strcmp_P(cb.command, PSTR("RdPotent")) == 0) {
      cb.BeginResponse();
      cb.WriteValue(value);
      cb.EndResponse();
      return true;
    }
    return false;
  }
  
  double StepValue(int steps) {
    return double(int(constrain(value, 0.0, 0.99999) * double(steps)));
  }
  
  Version version;
  double value;
  unsigned long last_sample;
};

#endif
