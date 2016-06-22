#include <Arduino.h>

#include "command_handler.h"

#include "component.h"
#include "component_driver.h"

CommandHandler::CommandHandler(ComponentDriver* component_driver, Component* blinker)
    : component_driver_(component_driver),
      blinker_(blinker) {  
}

CommandHandler::~CommandHandler() { }

void CommandHandler::HandleButton(int button, bool state) {
  if (button == 3 && state) {
    component_driver_->StopAndResetComponents();
    blinker_->Register(component_driver_);
    component_driver_->StartComponents();
    return;
  }
  if (button == 4 && state) {
    component_driver_->StopAndResetComponents();
    return;
  }
}
