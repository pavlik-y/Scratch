#ifndef DESK_CONTROLLER_H_
#define DESK_CONTROLLER_H_

#include "double_click_detector.h"

class DeskController {
 public:
  DeskController();
  void Setup(int line0_pin, int line1_pin, int line2_pin, int line3_pin);

  void SetCommand(int command);
  void ResetCommand(int command);

  void Tick();

  bool DoubleClickDetected(int button);
  bool ExternalInput();

 private:
  int ReadExternalInput();

  int line_pins_[4];
  int current_command_;
  int external_input_;

  DoubleClickDetector double_click_detectors_[2];
};

#endif  // DESK_CONTROLLER_H_