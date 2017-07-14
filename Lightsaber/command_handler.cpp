#include <Arduino.h>

#include "command_handler.h"

#include "component.h"
#include "component_driver.h"
#include "blinker.h"
#include "prefs.h"
#include "sensor_display.h"
#include "time_bar_display.h"

CommandHandler::CommandHandler(ComponentDriver* component_driver, Prefs* prefs,
    RTC_PCF8523* rtc,
    Component* blinker,
    Component* shock_flash,
    Component* digital_clock,
    SensorDisplay* sensor_display,
    Component* time_bar_display,
    Component* flashlight,
    Component* rainbow)
    : component_driver_(component_driver),
      prefs_(prefs),
      rtc_(rtc),
      blinker_(blinker),
      shock_flash_(shock_flash),
      digital_clock_(digital_clock),
      sensor_display_(sensor_display),
      time_bar_display_(time_bar_display),
      flashlight_(flashlight),
      rainbow_(rainbow),
      mode_(MODE_OFF) {
}

void CommandHandler::HandleButton(int button, bool pressed) {
  if (!pressed)
    return;
  int mode = mode_;
  int sub_mode = 1;

  switch (button) {
    case 5:
      mode = MODE_FLASHLIGHT;
      break;
    case 6:
      mode = MODE_OFF;
      break;
    case 7:
      mode = MODE_SENSORS;
      break;
    case 8:
      mode = MODE_PATTERN;
      break;
    case 1:
    case 2:
    case 3:
    case 4:
      sub_mode = button;
      break;
  }
  SwitchToMode(mode, sub_mode);
  mode_ = mode;
}

void CommandHandler::HandleColor(uint8_t r, uint8_t g, uint8_t b) {
  prefs_->SetColor(r, g, b);
}

void CommandHandler::HandleSetTime(const char* time_str) {
  const char* date_str = "Jan 01 1970";
  DateTime date_time(date_str, time_str);
  rtc_->adjust(date_time);
}

void CommandHandler::SwitchToMode(Mode mode, int sub_mode) {
  component_driver_->StopAndResetComponents();
  switch (mode) {
    case MODE_OFF:
      break;
    case MODE_FLASHLIGHT:
      if (sub_mode == 1)
        flashlight_->Register(component_driver_);
      else if (sub_mode == 2)
        rainbow_->Register(component_driver_);
      else
        shock_flash_->Register(component_driver_);
      break;
    case MODE_SENSORS:
      if (sub_mode == 4) {
        digital_clock_->Register(component_driver_);
        break;
      }
      sensor_display_->Register(component_driver_);
      if (sub_mode == 1)
        sensor_display_->SetSensorType(SensorDisplay::ACCELEROMETER);
      else if (sub_mode == 2)
        sensor_display_->SetSensorType(SensorDisplay::GYROSCOPE);
      else
        sensor_display_->SetSensorType(SensorDisplay::COMPASS);
      break;
    case MODE_PATTERN:
      if (sub_mode == 1) {
        blinker_->Register(component_driver_);
      } else {
        time_bar_display_->Register(component_driver_);
      }
      break;
  }
  component_driver_->StartComponents();
}

