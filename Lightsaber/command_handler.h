#ifndef COMMAND_HANDLER_H_
#define COMMAND_HANDLER_H_

#include <RTClib.h>

class Blinker;
class Component;
class ComponentDriver;
class Prefs;
class SensorDisplay;

class CommandHandler {
 public:
  CommandHandler(ComponentDriver* component_driver, Prefs* prefs, RTC_PCF8523* rtc, 
      Blinker* blinker, Component* shock_flash, Component* digital_clock, SensorDisplay* sensor_display);
  void HandleButton(int button, bool state);
  void HandleColor(uint8_t r, uint8_t g, uint8_t b);
  void HandleSetTime(const char * time_str);
  
 private:
  ComponentDriver* component_driver_;
  Prefs* prefs_;
  RTC_PCF8523* rtc_;
  Blinker* blinker_;
  Component* shock_flash_;
  Component* digital_clock_;
  SensorDisplay* sensor_display_;
};

#endif
