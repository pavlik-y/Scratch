#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <Adafruit_SSD1306.h>

class DeskController;

class Controller {
 public:
  Controller(DeskController* desk_controller, Adafruit_SSD1306* oled);
  void ButtonPressed(int button);
  void ButtonReleased(int button);
  void WordReceived(uint32_t value);

  void Tick();

 private:

  enum class State {
    IDLE,
    LATCHED,
  };

  DeskController* desk_controller_;
  Adafruit_SSD1306* oled_;

  State state_ = State::IDLE;
  uint32_t latched_start_time_ = 0;
  int latched_command_;
};

#endif  // CONTROLLER_H_