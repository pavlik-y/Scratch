#ifndef COMMAND_HANDLER_H_
#define COMMAND_HANDLER_H_

class Component;
class ComponentDriver;

class CommandHandler {
 public:
  CommandHandler(ComponentDriver* component_driver, Component* blinker);
  virtual ~CommandHandler();
  virtual void HandleButton(int button, bool state);
 private:
  ComponentDriver* component_driver_;
  Component* blinker_;
};

#endif
