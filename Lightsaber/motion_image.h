#ifndef MOTION_IMAGE_H_
#define MOTION_IMAGE_H_

#include <Adafruit_LSM9DS0.h>
#include <Adafruit_NeoPixel.h>

#include "component.h"

class MotionImage : public Component {
 public:
  MotionImage(Adafruit_NeoPixel* strip, Adafruit_LSM9DS0* sensor);

  void Stop() override;
  void Tick() override;

 private:  
  Adafruit_NeoPixel* strip_;
  Adafruit_LSM9DS0* sensor_;
};

#endif  // MOTION_IMAGE_H_
