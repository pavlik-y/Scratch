#ifndef COMPONENT_MANAGER_H_
#define COMPONENT_MANAGER_H_

#include "common.h"

class CommandBuffer;
class Component;
class Config;

class ComponentManager {
public:
  ComponentManager(int capacity);

  void RegisterComponent(Component* component);
  void Update();

  bool HandleCommand(CommandBuffer& cb);
  void ReadConfig(Config* config);

private:
  Component** components_;
  int size_;
  int capacity_;
};

#endif  // COMPONENT_MANAGER_H_
