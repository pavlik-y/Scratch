#include <Arduino.h>

#include "command_handler.h"

#include "component.h"
#include "component_driver.h"
#include "prefs.h"

CommandHandler::CommandHandler(ComponentDriver* component_driver, Prefs* prefs, RTC_PCF8523* rtc, 
    Component* blinker, Component* shock_flash, Component* digital_clock)
    : component_driver_(component_driver),
      prefs_(prefs),
      rtc_(rtc),
      blinker_(blinker),
      shock_flash_(shock_flash),
      digital_clock_(digital_clock) {
}

void CommandHandler::HandleButton(int button, bool state) {
  if (button == 1 && state) {
    component_driver_->StopAndResetComponents();
    shock_flash_->Register(component_driver_);
    component_driver_->StartComponents();
    return;
  }
  if (button == 2 && state) {
    component_driver_->StopAndResetComponents();
    digital_clock_->Register(component_driver_);
    component_driver_->StartComponents();
    return;
  }
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

void CommandHandler::HandleSetTime(const char* time_str) {
  const char* date_str = "Jan 01 1970";
  DateTime date_time(date_str, time_str);
  rtc_->adjust(date_time);
}

