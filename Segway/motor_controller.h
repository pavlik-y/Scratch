#ifndef MOTOR_CONTROLLER_H_
#define MOTOR_CONTROLLER_H_

#include <Arduino.h>

#include "common.h"
#include "component.h"

class Calibration;
class CommandBuffer;
class Config;
class FallDetector;
class MotorDriver;
class TiltController;

class MotorController : public Component {
public:
  void Setup(
      MotorDriver* motor_driver, TiltController* tilt_controller,
      FallDetector* fall_detector, Calibration* calibration);
  void ReadConfig(Config* config) override;
  void Update() override;
  bool HandleCommand(CommandBuffer& cb) override;

private:
  MotorDriver* motor_driver_;
  TiltController* tilt_controller_;
  Version tilt_controller_version_;
  FallDetector* fall_detector_;
  Version fall_detector_version_;
  Calibration* calibration_;
  Version calibration_version_;

  bool motor_enabled_;
  double left_power_;
  double right_power_;

  double left_zero_;
  double left_one_;

  double right_zero_;
  double right_one_;

  double turn_offset_;
};

#endif
