#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_

#include <Arduino.h>
#include <HardwarePWM.h>

class MotorDriver {
public:
  MotorDriver(HardwarePWM* pwm,
      int left_a, int left_b, int left_en,
      int right_a, int right_b, int right_en);

  void Setup();

  void SetPower(int left_dir, double left_power,
                int right_dir, double right_power);

  void SetPowerRaw(int left_dir, uint16_t left_power,
                   int right_dir, uint16_t right_power);

private:
  static const uint16_t kPwmMaxValue = 255;

  HardwarePWM* pwm_;
  int left_a_;
  int left_b_;
  int left_en_;
  int right_a_;
  int right_b_;
  int right_en_;
};

#endif  // MOTOR_DRIVER_H_
