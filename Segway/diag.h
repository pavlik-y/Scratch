#ifndef DIAG_H_
#define DIAG_H_

#include <Arduino.h>
// #include <SwRotaryEncoder.h>

#include "common.h"
#include "component.h"

class Accel;
class Gyro;
class Position;
class SensorFusion;

class Diag : public Component {
 public:
  Diag();
  void Setup(Gyro* gyro, Accel* accel, SensorFusion* sensor_fusion,
      Position* position);

  void Update() override;
  bool HandleCommand(CommandBuffer& cb) override;

 private:
  Accel* accel_;
  Gyro* gyro_;
  Version gyro_version_;
  SensorFusion* sensor_fusion_;
  Position* position_;

  double gyro_angle_ = 0.0;
  int32_t gyro_int_angle_ = 0;
  uint32_t sample_count = 0;

  bool enabled_ = true;

  unsigned long last_sample_time_micros_ = 0;
};

#endif  // DIAG_H_