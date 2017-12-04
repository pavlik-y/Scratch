#ifndef DIAG_H_
#define DIAG_H_

#include <Arduino.h>
#include "common.h"
#include "component.h"

class Gyro;

class Diag : public Component {
 public:
  Diag();
  void Setup(Gyro* gyro);

  void Update() override;

 private:
  Gyro* gyro_;

  unsigned long last_sample_time_;
};

#endif  // DIAG_H_