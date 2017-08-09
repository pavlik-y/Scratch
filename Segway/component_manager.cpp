#include "component_manager.h"

#include <Arduino.h>

#include "command_buffer.h"
#include "component.h"

bool Component::HandleCommand(CommandBuffer& cb) {
  return false;
}

void Component::ReadConfig(Config* config) {}

ComponentManager::ComponentManager(int capacity)
    : capacity_(capacity),
      size_(0) {
    components_ = new Component*[capacity];
    if (components_ == NULL)
      halt(100);
}

void ComponentManager::RegisterComponent(Component* component) {
  if (size_ >= capacity_)
    halt(100);
  components_[size_] = component;
  ++size_;
}

void ComponentManager::Update() {
  for (int i = 0; i < size_; ++i) {
    components_[i]->Update();
  }
}

bool ComponentManager::HandleCommand(CommandBuffer& cb) {
  for (int i = 0; i < size_; ++i) {
    if (components_[i]->HandleCommand(cb))
      return true;
  }
  return false;
}

void ComponentManager::ReadConfig(Config* config) {
  for (int i = 0; i < size_; ++i) {
    components_[i]->ReadConfig(config);
  }
}
