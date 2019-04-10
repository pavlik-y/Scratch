#ifndef DOUBLE_CLICK_DETECTOR_H_
#define DOUBLE_CLICK_DETECTOR_H_

#include <Arduino.h>

class DoubleClickDetector {
 public:
  void Update(bool button_pressed, uint32_t time_ms);
  bool DoubleClickDetected() const;

 private:
  enum class State {
    IDLE,
    FIRST_PRESS,
    FIRST_RELEASE,
    SECOND_PRESS,
    SECOND_RELEASE,
  };

  void SetState(State new_state, uint32_t time_ms);

  State state_ = State::IDLE;
  uint32_t last_state_change_time_ms_ = 0;
  bool double_click_detected_ = false;
};

#endif  // DOUBLE_CLICK_DETECTOR_H_
