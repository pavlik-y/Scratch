#include "motor_encoder.h"

MotorEncoder::MotorEncoder(int a_pin, int b_pin)
    : a_pin_(a_pin),
      b_pin_(b_pin) {}

void MotorEncoder::Setup() {
  pinMode(a_pin_, INPUT);
  pinMode(b_pin_, INPUT);
  mask_ = 1 << a_pin_;
  // last_a_ = PIND & mask_;
  pos = 0;
  errors = 0;
}

void MotorEncoder::Read() {
  // byte value = PIND;
  // int increment = 1;
  // if ((value & mask_) == last_a_) {
  //   // By the time we read value of A it is the same as last time. We likely
  //   // missed sample. Record error and adjust previous value by 2.
  //   ++errors;
  //   increment = 2;
  // }
  // // This code relies on the fact that A and B are connected 2 pins apart, but
  // // it is not asserted anywhere.
  // if ((value ^ (value >> 2)) & mask_)
  //   pos += increment;
  // else
  //   pos -= increment;
  // last_a_ = value & mask_;
}
