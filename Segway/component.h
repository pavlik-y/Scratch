#ifndef COMPONENT_H_
#define COMPONENT_H_

class CommandBuffer;
class Config;

class Component {
public:
  virtual void Update() = 0;
  virtual bool HandleCommand(CommandBuffer& cb);
  virtual void ReadConfig(Config* config);
};

#endif  // COMPONENT_H_