#include <Arduino.h>

#include "desk_controller.h"

DeskController::DeskController()
    : current_command_(0), external_input_(0) {}

void DeskController::Setup(
    int line0_pin, int line1_pin, int line2_pin, int line3_pin) {
  line_pins_[0] = line0_pin;
  line_pins_[1] = line1_pin;
  line_pins_[2] = line2_pin;
  line_pins_[3] = line3_pin;
  for (int i = 0; i < 4; i++)
    pinMode(line_pins_[i], INPUT);
}

void DeskController::SetCommand(int command) {
  Serial.println("SetCommand");
  if (external_input_ != 0 || current_command_ != 0)
    return;

  for (int i = 0; i < 4; i++) {
    if (command & (1 << i)) {
      Serial.printf("Set pin:%d\n", i);
      pinMode(line_pins_[i], OUTPUT);
      digitalWrite(line_pins_[i], HIGH);
    }
  }
  current_command_ = command;
}

void DeskController::ResetCommand(int command) {
  if (current_command_ != command)
    return;
  for (int i = 0; i < 4; i++) {
    digitalWrite(line_pins_[i], LOW);
    pinMode(line_pins_[i], INPUT);
  }
  current_command_ = 0;
}

int DeskController::ReadExternalInput() {
  int external_input = 0;
  for (int i = 0; i < 4; i++) {
    if ((current_command_ & (1 << i)) == 0) {
      if (digitalRead(line_pins_[i]) == HIGH)
        external_input |= 1<<i;
    }
  }
  return external_input;
}

void DeskController::Tick() {
  int external_input = ReadExternalInput();
  if (external_input != 0 && current_command_ != 0) {
    ResetCommand(current_command_);
    external_input = ReadExternalInput();
  }
  if (external_input != external_input_) {
    Serial.printf("ExternalInput:%x\n", external_input);
    // ExternalInputChanged(external_input);
    external_input_ = external_input;
  }
  uint32_t now = millis();
  double_click_detectors_[0].Update((external_input_ & 1) != 0, now);
  double_click_detectors_[1].Update((external_input_ & 2) != 0, now);
}

bool DeskController::DoubleClickDetected(int button) {
  return double_click_detectors_[button].DoubleClickDetected();
}

bool DeskController::ExternalInput() {
  return external_input_ != 0;
}