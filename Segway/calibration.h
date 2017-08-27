#ifndef CALIBRATION_H_
#define CALIBRATION_H_

#include <Arduino.h>

#include "common.h"
#include "component.h"

class Accel;
class Gyro;
class MotorDriver;
class SensorFusion;

class Calibration : public Component{
public:

  void Setup(
      Accel* accel, Gyro* gyro, SensorFusion* sensor_fusion,
      MotorDriver* motor_driver);
  void Update() override;
  bool HandleCommand(CommandBuffer& cb) override;

  Version version;
  int state;
  double avg_accel_angle;
  double accel_angle_div;
  double avg_lambda;
  unsigned long calibration_start_time;
  unsigned long calibration_duration;
  long gyro_angle;
  long gyro_sample_count;
  double gyro_bias;

private:
  Accel* accel_;
  Gyro* gyro_;
  Version gyro_version_;
  SensorFusion* sensor_fusion_;
  MotorDriver* motor_driver_;
};

#endif  // CALIBRATION_H_
