#ifndef COMPONENT_H_
#define COMPONENT_H_

class Component {
 public:
  virtual ~Component();

  virtual void Start();
  virtual void Tick();
  virtual void Stop();
  
};
#endif  // COMPONENT_H_
