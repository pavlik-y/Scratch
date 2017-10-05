#include "motor.h"

// The motor driver can handle a PWM frequency up to 20kHz.
const uint16_t PWM_FREQUENCY = 20000;
// The frequency is given by F_CPU/(2*N*ICR) - where N is the prescaler,
// prescaling is used so the frequency is given by
// F_CPU/(2*ICR) - ICR = F_CPU/PWM_FREQUENCY/2
const uint16_t PWMVALUE = F_CPU / PWM_FREQUENCY / 2;


MotorDriver::MotorDriver(int left_A, int left_B, int left_EN,
                         int right_A, int right_B, int right_EN)
    : left_A_(left_A),
      left_B_(left_B),
      left_EN_(left_EN),
      right_A_(right_A),
      right_B_(right_B),
      right_EN_(right_EN) {}

void MotorDriver::Setup() {
  pinMode(left_A_, OUTPUT);
  pinMode(left_B_, OUTPUT);
  pinMode(left_EN_, OUTPUT);
  pinMode(right_A_, OUTPUT);
  pinMode(right_B_, OUTPUT);
  pinMode(right_EN_, OUTPUT);
}

void MotorDriver::SetupTimer1() {
  // Set PWM frequency to 20kHz - see the datasheet
  // http://www.atmel.com/Images/Atmel-8272-8-bit-AVR-microcontroller-ATmega164A_PA-324A_PA-644A_PA-1284_P_datasheet.pdf
  // page 129-139.
  // Set up PWM, Phase and Frequency Correct on pin 18 (OC1A) & pin 17 (OC1B)
  // with ICR1 as TOP using Timer1

  // Set PWM Phase and Frequency Correct with ICR1 as TOP and no prescaling.
  // TCCR1B = (1 << WGM13) | (1 << CS10);
  // ICR1 is the TOP value - this is set so the frequency is equal to 20kHz.
  // ICR1 = PWMVALUE;

  /* Enable PWM on pin 18 (OC1A) & pin 17 (OC1B) */
  // Clear OC1A/OC1B on compare match when up-counting
  // Set OC1A/OC1B on compare match when down-counting
  // TCCR1A = (1 << COM1A1) | (1 << COM1B1);
}

void MotorDriver::SetPower(int left_dir, double left_power,
                           int right_dir, double right_power) {
  uint16_t duty_cycle;

  if (left_dir < 0) {
    digitalWrite(left_A_, HIGH);
    digitalWrite(left_B_, LOW);
  } else {
    digitalWrite(left_A_, LOW);
    digitalWrite(left_B_, HIGH);
  }
  duty_cycle = left_power * double(PWMVALUE);
  // OCR1A = duty_cycle;

  if (right_dir < 0) {
    digitalWrite(right_A_, HIGH);
    digitalWrite(right_B_, LOW);
  } else {
    digitalWrite(right_A_, LOW);
    digitalWrite(right_B_, HIGH);
  }
  duty_cycle = right_power * double(PWMVALUE);
  // OCR1B = duty_cycle;
}
