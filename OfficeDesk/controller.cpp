#include "controller.h"

#include <Arduino.h>

#include "desk_controller.h"

Controller::Controller(DeskController* desk_controller)
    : desk_controller_(desk_controller) {}

void Controller::ButtonPressed(int button) {
  Serial.printf("ButtonPressed:%x\n", button);
  desk_controller_->SetCommand(button);
}

void Controller::ButtonReleased(int button) {
  Serial.printf("ButtonReleased:%x\n", button);
  desk_controller_->ResetCommand(button);
}