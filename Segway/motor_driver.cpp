#include "motor_driver.h"

MotorDriver::MotorDriver(HardwarePWM* pwm,
    int left_a, int left_b, int left_en,
    int right_a, int right_b, int right_en)
    : pwm_(pwm),
      left_a_(left_a),
      left_b_(left_b),
      left_en_(left_en),
      right_a_(right_a),
      right_b_(right_b),
      right_en_(right_en) {}

void MotorDriver::Setup() {
  pwm_->begin();
  pwm_->setMaxValue(kPwmMaxValue);
  pwm_->setClockDiv(PWM_PRESCALER_PRESCALER_DIV_1); // default : freq = 16Mhz
  pwm_->addPin(left_en_);
  pwm_->addPin(right_en_);

  pinMode(left_a_, OUTPUT);
  pinMode(left_b_, OUTPUT);
  pinMode(right_a_, OUTPUT);
  pinMode(right_b_, OUTPUT);
  SetPowerRaw(0, 0, 0, 0);
}

void MotorDriver::SetPower(int left_dir, double left_power,
                           int right_dir, double right_power) {
  uint16_t left_raw = constrain(left_power, 0.0, 1.0) * double(kPwmMaxValue);
  uint16_t right_raw = constrain(right_power, 0.0, 1.0) * double(kPwmMaxValue);
  SetPowerRaw(left_dir, left_raw, right_dir, right_raw);
}

void MotorDriver::SetPowerRaw(int left_dir, uint16_t left_power,
                              int right_dir, uint16_t right_power) {
  digitalWrite(left_a_, left_dir > 0 ? HIGH : LOW);
  digitalWrite(left_b_, left_dir < 0 ? HIGH : LOW);
  pwm_->writePin(left_en_, left_power);

  digitalWrite(right_a_, right_dir > 0 ? HIGH : LOW);
  digitalWrite(right_b_, right_dir < 0 ? HIGH : LOW);
  pwm_->writePin(right_en_, right_power);
}
