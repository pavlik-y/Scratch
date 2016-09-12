#include <Arduino.h>

#include "command_handler.h"

#include "component.h"
#include "component_driver.h"
#include "prefs.h"

CommandHandler::CommandHandler(ComponentDriver* component_driver, Prefs* prefs, Component* blinker)
    : component_driver_(component_driver),
      prefs_(prefs),
      blinker_(blinker) {  
}

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

void CommandHandler::HandleColor(uint8_t r, uint8_t g, uint8_t b) {
  Serial.println(F("Handle color"));
  prefs_->SetColor(r, g, b);
}

