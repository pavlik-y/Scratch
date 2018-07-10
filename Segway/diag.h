#ifndef DIAG_H_
#define DIAG_H_

#include <Arduino.h>
#include <SwRotaryEncoder.h>

#include "common.h"
#include "component.h"

class Accel;
class Gyro;
class MotorController;
class Position;
class SensorFusion;
class TiltController;

class Diag : public Component {
 public:
  Diag();
  void Setup(Gyro* gyro, Accel* accel, SensorFusion* sensor_fusion,
      Position* position,
      SwRotaryEncoder* left_encoder, SwRotaryEncoder* right_encoder,
      TiltController* tilt_controller, MotorController* motor_controller);

  void Update() override;
  void ReadConfig(Config* config) override;

 private:
  enum class Mode : int {
    DISABLED = 0,
    ACCEL = 1,
    GYRO = 2,
    TILT_CONTROLLER = 3,
  };

  void DumpMeasurements();

  Accel* accel_;
  Gyro* gyro_;
  SensorFusion* sensor_fusion_;
  Position* position_;
  SwRotaryEncoder* left_encoder_;
  SwRotaryEncoder* right_encoder_;
  TiltController* tilt_controller_;
  MotorController* motor_controller_;

  double gyro_angle_ = 0.0;
  int32_t gyro_int_angle_ = 0;

  Mode mode_ = Mode::DISABLED;

  unsigned long last_sample_time_micros_ = 0;
};

#endif  // DIAG_H_