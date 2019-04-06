#ifndef DESK_CONTROLLER_H_
#define DESK_CONTROLLER_H_

class DeskController {
 public:
  DeskController();
  void Setup(int line0_pin, int line1_pin, int line2_pin, int line3_pin);

  void SetCommand(int command);
  void ResetCommand(int command);

  void Tick();

 private:
  int ReadExternalInput();

  int line_pins_[4];
  int current_command_;
  int external_input_;
};

#endif  // DESK_CONTROLLER_H_