#ifndef MOTOR_CONTROLLER_H_
#define MOTOR_CONTROLLER_H_

#include <Arduino.h>

#include "common.h"
#include "component.h"

class Config;
class FallDetector;
class MotorDriver;
class TiltController;


struct PowerAndDirection {
  double power;
  int direction;
};

class VelocityToPowerConverter {
 public:
  PowerAndDirection VelocityToPowerAndDirection(double velocity);
  void SetParameters(
      double min_velocity, double max_velocity,
      double min_power, double max_power);

 private:
  double min_velocity_;
  double min_power_;
  double ratio_;
};

class MotorController : public Component {
 public:
  void Setup(MotorDriver* motor_driver, TiltController* tilt_controller,
             FallDetector* fall_detector);

  void ReadConfig(Config* config) override;
  void Update() override;

  double left_power = 0;
  double right_power = 0;

 private:
  void ReadConverterParameters(Config* config,
      VelocityToPowerConverter* converter,
      const char* min_vel_name, const char* max_vel_name,
      const char* min_power_name, const char* max_power_name);

  void SetVelocity(double left_velocity, double right_velocity);

  MotorDriver* motor_driver_;
  TiltController* tilt_controller_;
  Version tilt_controller_version_;
  FallDetector* fall_detector_;
  Version fall_detector_version_;

  VelocityToPowerConverter left_converter_;
  VelocityToPowerConverter right_converter_;
  bool motor_enabled_ = false;
};

#endif
