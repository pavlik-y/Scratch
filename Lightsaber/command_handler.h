#ifndef COMMAND_HANDLER_H_
#define COMMAND_HANDLER_H_

#include <RTClib.h>

class Component;
class ComponentDriver;
class Prefs;

class CommandHandler {
 public:
  CommandHandler(ComponentDriver* component_driver, Prefs* prefs, RTC_PCF8523* rtc, 
      Component* blinker, Component* shock_flash, Component* digital_clock);
  void HandleButton(int button, bool state);
  void HandleColor(uint8_t r, uint8_t g, uint8_t b);
  void HandleSetTime(const char * time_str);
  
 private:
  ComponentDriver* component_driver_;
  Prefs* prefs_;
  RTC_PCF8523* rtc_;
  Component* blinker_;
  Component* shock_flash_;
  Component* digital_clock_;
};

#endif
