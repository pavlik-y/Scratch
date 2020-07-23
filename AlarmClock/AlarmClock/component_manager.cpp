#include "component_manager.h"

#include "common.h"
#include "component.h"

ComponentManagerBase::ComponentManagerBase(
    int capacity, Component** component_storage)
  : capacity_(capacity),
    components_(component_storage) {
  for (int i = 0; i < capacity_; i++)
  components_[i] = nullptr;
}

void ComponentManagerBase::RegisterComponent(Component* component) {
  int index;
  for (index = 0; index < capacity_; index++)
    if (components_[index] == nullptr)
      break;
  if (index == capacity_)
    halt(0x11, "Too many components");
  components_[index] = component;
  component->Setup();
}

void ComponentManagerBase::UnregisterComponent(Component* component) {
  for (int i = 0; i < capacity_; i++) {
    if (components_[i] == component) {
      components_[i] = nullptr;
      return;
    }
  }
  halt(0x12, "Component not found");
}

void ComponentManagerBase::Tick() {
  for (int i = 0; i < capacity_; i++) {
    if (components_[i] != nullptr)
      components_[i]->Tick();
  }
}