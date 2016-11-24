#ifndef MAGIC_WAND_H_
#define MAGIC_WAND_H_

#include <Adafruit_LSM9DS0.h>
#include <Adafruit_NeoPixel.h>

#include "component.h"

class MagicWand : public Component {
 public:
  MagicWand(Adafruit_NeoPixel* strip, Adafruit_LSM9DS0* sensor);

  void Stop() override;
  void Tick() override;

 private:  
  Adafruit_NeoPixel* strip_;
  Adafruit_LSM9DS0* sensor_;
};

#endif  // MAGIC_WAND_H_
