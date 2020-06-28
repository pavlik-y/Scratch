#ifndef COMPONENT_MANAGER_H_
#define COMPONENT_MANAGER_H_

class Component;

class ComponentManagerBase {
 public:
  ComponentManagerBase(int capacity, Component** component_storage);

  void RegisterComponent(Component* component);
  void UnregisterComponent(Component* component);

  void Tick();

 private:
  const int capacity_;
  Component** const components_;
};

template<int capacity> class ComponentManager :
    public ComponentManagerBase {
 public:
  ComponentManager() : ComponentManagerBase(capacity, components_storage_) {}
 private:
  Component* components_storage_[capacity];
};

#endif  // COMPONENT_MANAGER_H_