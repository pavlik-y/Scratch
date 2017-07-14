#ifndef COMMAND_HANDLER_H_
#define COMMAND_HANDLER_H_

#include <RTClib.h>

class Component;
class ComponentDriver;
class Prefs;
class SensorDisplay;

class CommandHandler {
 public:
  CommandHandler(ComponentDriver* component_driver, Prefs* prefs,
      RTC_PCF8523* rtc,
      Component* blinker,
      Component* shock_flash,
      Component* digital_clock,
      SensorDisplay* sensor_display,
      Component* time_bar_display,
      Component* flashlight,
      Component* rainbow);
  void HandleButton(int button, bool state);
  void HandleColor(uint8_t r, uint8_t g, uint8_t b);
  void HandleSetTime(const char * time_str);

 private:
  enum Mode {
    MODE_OFF,
    MODE_FLASHLIGHT,
    MODE_SENSORS,
    MODE_PATTERN
  };

  void SwitchToMode(Mode mode, int sub_mode);

  ComponentDriver* component_driver_;
  Prefs* prefs_;
  RTC_PCF8523* rtc_;
  Component* blinker_;
  Component* shock_flash_;
  Component* digital_clock_;
  SensorDisplay* sensor_display_;
  Component* time_bar_display_;
  Component* flashlight_;
  Component* rainbow_;

  Mode mode_;
};

#endif
