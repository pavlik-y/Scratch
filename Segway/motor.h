#ifndef MOTOR_H_
#define MOTOR_H_

#include <Arduino.h>

class MotorDriver {
public:
  MotorDriver(int left_A, int left_B, int left_EN,
              int right_A, int right_B, int right_EN);
  void Setup();
  void SetupTimer1();
  void SetPower(int left_dir, double left_power,
                int right_dir, double right_power);

private:
  int left_A_;
  int left_B_;
  int left_EN_;
  int right_A_;
  int right_B_;
  int right_EN_;
};

#endif  // MOTOR_H_
