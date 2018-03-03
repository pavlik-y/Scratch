#ifndef DIAG_H_
#define DIAG_H_

#include <Arduino.h>
#include "common.h"
#include "component.h"

class Accel;
class Gyro;

class Diag : public Component {
 public:
  Diag();
  void Setup(Gyro* gyro, Accel* accel);

  void Update() override;
  bool HandleCommand(CommandBuffer& cb) override;

 private:
  Accel* accel_;
  Gyro* gyro_;
  Version gyro_version_;

  double gyro_angle_ = 0.0;

  bool enabled_ = true;

  unsigned long last_sample_time_;
};

#endif  // DIAG_H_