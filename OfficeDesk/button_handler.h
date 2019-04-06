#ifndef BUTTON_HANDLER_H_
#define BUTTON_HANDLER_H_

class Controller;

class ButtonHandler {
 public:
  ButtonHandler(Controller* controller);

  void Setup(int a_pin, int b_pin, int c_pin);
  void Tick();

 private:
  enum class State {
    OPEN,
    PRESSED,
    LATCHED
  };

  int pins_[3];
  Controller* controller_;
  State state_;
  int pressed_button_;

};

#endif  // BUTTON_HANDLER_H_
