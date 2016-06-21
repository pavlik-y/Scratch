#include "component_driver.h"

#include "component.h"

ComponentDriver::ComponentDriver() {
  for (int i = 0; i < COMPONENT_COUNT; i++)
    components_[i] = nullptr;
}

void ComponentDriver::Tick() {
  for (int i = 0; i < COMPONENT_COUNT; i++)
    if (components_[i] != nullptr)
      components_[i]->Tick();
}

void ComponentDriver::RegisterComponent(Component* component) {
  for (int i = 0; i < COMPONENT_COUNT; i++) {
    if (components_[i] == component)
      return;
    if (components_[i] == nullptr) {
      components_[i] = component;
      return;
    }
  }
  // halt();
}

void ComponentDriver::StartComponents() {
  for (int i = 0; i < COMPONENT_COUNT; i++)
    if (components_[i] != nullptr)
      components_[i]->Start();
}

void ComponentDriver::StopAndResetComponents() {
  int i = COMPONENT_COUNT;
  do {
    i--;
    if (components_[i] != nullptr)
      components_[i]->Stop();
      components_[i] = nullptr;
  } while (i > 0);
}

