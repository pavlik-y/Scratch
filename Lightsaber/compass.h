#ifndef COMPASS_H_
#define COMPASS_H_

#include <Adafruit_LSM9DS0.h>
#include <Adafruit_NeoPixel.h>

#include "component.h"

class Compass : public Component {
 public:
  Compass(Adafruit_NeoPixel* strip, Adafruit_LSM9DS0* sensor);

  void Start() override;
  void Stop() override;
  void Tick() override;
  
 private:
  Adafruit_NeoPixel* strip_;
  Adafruit_LSM9DS0* sensor_;
  unsigned long last_sample_time_;
};

#endif // COMPASS_H_
