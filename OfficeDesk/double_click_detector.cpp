#include "double_click_detector.h"

const uint32_t kClickThreshold = 300;

void DoubleClickDetector::Update(bool button_pressed, uint32_t time_ms) {
  if (state_ != State::IDLE &&
      time_ms - last_state_change_time_ms_ >= kClickThreshold) {
    SetState(State::IDLE, time_ms);
    return;
  }
  switch (state_) {
    case State::IDLE:
      if (button_pressed)
        SetState(State::FIRST_PRESS, time_ms);
      break;
    case State::FIRST_PRESS:
      if (!button_pressed)
        SetState(State::FIRST_RELEASE, time_ms);
      break;
    case State::FIRST_RELEASE:
      if (button_pressed)
        SetState(State::SECOND_PRESS, time_ms);
      break;
    case State::SECOND_PRESS:
      if (!button_pressed) {
        double_click_detected_ = true;
        SetState(State::SECOND_RELEASE, time_ms);
      }
      break;
    case State::SECOND_RELEASE:
      double_click_detected_ = false;
      SetState(State::IDLE, time_ms);
      break;
  }
}

bool DoubleClickDetector::DoubleClickDetected() const {
  return double_click_detected_;
}

void DoubleClickDetector::SetState(State new_state, uint32_t time_ms) {
  Serial.printf("dblclk:%d\n", new_state);
  last_state_change_time_ms_ = time_ms;
  state_ = new_state;
}