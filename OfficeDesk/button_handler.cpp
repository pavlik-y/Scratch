#include <Arduino.h>

#include "button_handler.h"

#include "controller.h"

namespace {

// {PAV} Pass button map from outside
const int kButtonMap[3] = {4, 3, 8};

}  // namespace

ButtonHandler::ButtonHandler(Controller* controller)
    : controller_(controller),
      state_(State::OPEN),
      pressed_button_(0) {}

void ButtonHandler::Setup(int a_pin, int b_pin, int c_pin) {
  pinMode(a_pin, INPUT_PULLUP);
  pinMode(b_pin, INPUT_PULLUP);
  pinMode(c_pin, INPUT_PULLUP);
  pins_[0] = a_pin;
  pins_[1] = b_pin;
  pins_[2] = c_pin;
}

void ButtonHandler::Tick() {
  int pressed_button = -1; // -1: none, -2:multiple, >=0: button number
  for (int i = 0; i < 3; i++) {
    int level = digitalRead(pins_[i]);
    if (level == LOW) {
      if (pressed_button == -1)
        pressed_button = i;
      else if (pressed_button >= 0)
        pressed_button = -2;
    }
  }

  switch (state_) {
    case State::OPEN:
      if (pressed_button >= 0) {
        controller_->ButtonPressed(kButtonMap[pressed_button]);
        pressed_button_ = pressed_button;
        Serial.println("state pressed");
        state_ = State::PRESSED;
      } else if (pressed_button == -2) {
        Serial.println("state latched");
        state_ = State::LATCHED;
      }
      break;
    case State::PRESSED:
      if (pressed_button == pressed_button_)
        break;
      Serial.println("Button released");
      controller_->ButtonReleased(kButtonMap[pressed_button_]);
      if (pressed_button == -1) {
        Serial.println("state open");
        state_ = State::OPEN;
      } else {
        Serial.println("state latched");
        state_ = State::LATCHED;
      }
      break;
    case State::LATCHED:
      if (pressed_button == -1) {
        Serial.println("state open");
        state_ = State::OPEN;
      }
      break;
  }
}
