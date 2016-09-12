#ifndef COMMAND_HANDLER_H_
#define COMMAND_HANDLER_H_

class Component;
class ComponentDriver;
class Prefs;

class CommandHandler {
 public:
  CommandHandler(ComponentDriver* component_driver, Prefs* prefs, Component* blinker);
  void HandleButton(int button, bool state);
  void HandleColor(uint8_t r, uint8_t g, uint8_t b);
  
 private:
  ComponentDriver* component_driver_;
  Prefs* prefs_;
  Component* blinker_;
};

#endif
