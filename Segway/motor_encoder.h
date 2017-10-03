#ifndef MOTOR_ENCODER_H_
#define MOTOR_ENCODER_H_

#include <Arduino.h>

class MotorEncoder {
public:
  MotorEncoder(int a_pin, int b_pin);

  void Setup();
  void Read();

  long pos;
  long errors;

private:
  int a_pin_;
  int b_pin_;
  byte mask_;
  byte last_a_;
};

#endif  // MOTOR_ENCODER_H_
