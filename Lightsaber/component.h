#ifndef COMPONENT_H_
#define COMPONENT_H_

class ComponentDriver;

class Component {
 public:
  virtual ~Component();

  virtual void Register(ComponentDriver* component_driver);
  virtual void Start();
  virtual void Tick();
  virtual void Stop();
  
};
#endif  // COMPONENT_H_
