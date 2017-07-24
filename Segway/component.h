#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <Arduino.h>
#include "common.h"
#include "command_buffer.h"

class Config;

class Component {
public:
  virtual void Update() = 0;
  virtual bool HandleCommand(CommandBuffer& cb) {
    return false;
  }
  virtual void ReadConfig(Config* config) {
  }
};

class ComponentManager {
public:
  ComponentManager(int capacity) {
    capacity_ = capacity;
    size_ = 0;
    components_ = new Component*[capacity];
    if (components_ == NULL)
      halt(100);
  }

  void RegisterComponent(Component* component) {
    if (size_ >= capacity_)
      halt(100);
    components_[size_] = component;
    ++size_;
  }

  void Update() {
    for (int i = 0; i < size_; ++i) {
      components_[i]->Update();
    }
  }

  bool HandleCommand(CommandBuffer& cb) {
    for (int i = 0; i < size_; ++i) {
      if (components_[i]->HandleCommand(cb))
        return true;
    }
    return false;
  }

  void ReadConfig(Config* config) {
    for (int i = 0; i < size_; ++i) {
      components_[i]->ReadConfig(config);
    }
  }

private:
  Component** components_;
  int size_;
  int capacity_;
};

#endif
