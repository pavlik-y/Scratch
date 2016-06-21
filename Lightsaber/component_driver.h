#ifndef COMPONENT_DRIVER_H_
#define COMPONENT_DRIVER_H_

class Component;

class ComponentDriver {
 public:
  enum {
    COMPONENT_COUNT = 8  
  };
  
  ComponentDriver();

  void Tick();

  void RegisterComponent(Component* component);

  void StartComponents();
  void StopAndResetComponents();
  
 private:
  Component* components_[COMPONENT_COUNT];
};
#endif  // COMPONENT_DRIVER_H_
