#include <Arduino.h>

#include "command_handler.h"

#include "blinker.h"
#include "component.h"
#include "component_driver.h"
#include "prefs.h"
#include "sensor_display.h"

CommandHandler::CommandHandler(ComponentDriver* component_driver, Prefs* prefs, RTC_PCF8523* rtc, 
    Blinker* blinker, Component* shock_flash, Component* digital_clock, SensorDisplay* sensor_display)
    : component_driver_(component_driver),
      prefs_(prefs),
      rtc_(rtc),
      blinker_(blinker),
      shock_flash_(shock_flash),
      digital_clock_(digital_clock),
      sensor_display_(sensor_display) {
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
    blinker_->SetPredefinedPattern(0);
    blinker_->Register(component_driver_);
    component_driver_->StartComponents();
    return;
  }
  if (button == 6 && state) {
    component_driver_->StopAndResetComponents();
    return;
  }
  if (button == 7 && state) {
    component_driver_->StopAndResetComponents();
    sensor_display_->Register(component_driver_);
    sensor_display_->SetSensorType(SensorDisplay::COMPASS);
    component_driver_->StartComponents();
    return;
    
  }
  if (button == 8 && state) {
    component_driver_->StopAndResetComponents();
    sensor_display_->Register(component_driver_);
    sensor_display_->SetSensorType(SensorDisplay::ACCELEROMETER);
    component_driver_->StartComponents();
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

