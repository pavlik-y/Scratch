#include "component.h"

#include "component_driver.h"

void Component::Register(ComponentDriver* component_driver) {
  component_driver->RegisterComponent(this);
}

void Component::Start() {
}

void Component::Tick() {
}

void Component::Stop() {
}

