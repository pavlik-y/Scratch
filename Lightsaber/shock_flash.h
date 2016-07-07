#ifndef SHOCK_FLASH_H_
#define SHOCK_FLASH_H_

#include "component.h"

class ComponentDriver;

class ShockFlash : public Component {
 public:
  ShockFlash();
  void Register(ComponentDriver* component_driver) override;
  void Start() override;
  void Tick() override;
  
};
#endif  // SHOCK_FLASH_H_
