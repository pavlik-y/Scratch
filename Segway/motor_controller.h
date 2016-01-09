#ifndef MOTOR_CONTROLLER_H_
#define MOTOR_CONTROLLER_H_

#include <Arduino.h>

#include "common.h"
#include "component.h"
#include "fall_detector.h"
#include "ir.h"
#include "motor.h"
#include "tilt_controller.h"

class MotorController : public Component {
public:
  void Setup(MotorDriver* motor_driver, TiltController* tilt_controller, FallDetector* fall_detector, Calibration* calibration, IR* ir) {
    motor_driver_ = motor_driver;
    tilt_controller_ = tilt_controller;
    tilt_controller_version_ = tilt_controller_->version;
    fall_detector_ = fall_detector;
    fall_detector_version_ = fall_detector_->version;
    calibration_ = calibration;
    calibration_version_ = calibration_->version;
    ir_ = ir;
    ir_version_ = ir_->version;
    left_zero_ = 0;
    left_one_ = 0;
    right_zero_ = 0;
    right_one_ = 0;
  }
  
  virtual void ReadConfig(Config* config) {
    left_zero_ = config->ReadFloat_P(kMotor_LeftZero);
    left_one_ = config->ReadFloat_P(kMotor_LeftOne);
    right_zero_ = config->ReadFloat_P(kMotor_RightZero);
    right_one_ = config->ReadFloat_P(kMotor_RightOne);
    turn_offset_ = config->ReadFloat_P(kMotor_TurnOffset);
    motor_enabled_ = config->ReadFloat_P(kMotor_Enabled) > 0.5;
  }
  
  virtual void Update() {
    if (tilt_controller_version_ == tilt_controller_->version && 
        fall_detector_version_ == fall_detector_->version &&
        calibration_version_ == calibration_->version &&
        ir_version_ == ir_->version) 
       return;
    tilt_controller_version_ = tilt_controller_->version;
    fall_detector_version_ = fall_detector_->version;
    calibration_version_ = calibration_->version;
    ir_version_ = ir_->version;

    if (!fall_detector_->standing || calibration_->state != 2 || !motor_enabled_) {
      motor_driver_->SetPower(1, 0, 1, 0);
      return;
    }
    
    double left_power = tilt_controller_->power;
    double right_power = tilt_controller_->power;
    if (ir_->command == IR::Left) {
      left_power += turn_offset_;
      right_power -= turn_offset_;
    } else if (ir_->command == IR::Right) {
      left_power -= turn_offset_;
      right_power += turn_offset_;
    }
    left_power = constrain(left_power, -1.0, 1.0);
    right_power = constrain(right_power, -1.0, 1.0);
    
    int left_direction = 1;
    int right_direction = 1;
    if (left_power < 0) {
      left_power = -left_power;
      left_direction = -1;
    }
    if (right_power < 0) {
      right_power = -right_power;
      right_direction = -1;
    }

    left_power_ = left_zero_ + left_power * (left_one_ - left_zero_);
    right_power_ = right_zero_ + right_power * (right_one_ - right_zero_);
    motor_driver_->SetPower(left_direction, left_power_, right_direction, right_power_);
  }

  virtual bool HandleCommand(CommandBuffer& cb) {
    if (strcmp_P(cb.command, PSTR("RdMotor")) == 0) {
      cb.BeginResponse();
      cb.WriteValue(tilt_controller_->power);
      cb.WriteValue(left_power_);
      cb.WriteValue(right_power_);
      cb.EndResponse();
      return true;
    }
    return false;
  }
  
private:
  MotorDriver* motor_driver_;
  TiltController* tilt_controller_;
  Version tilt_controller_version_;
  FallDetector* fall_detector_;
  Version fall_detector_version_;
  Calibration* calibration_;
  Version calibration_version_;
  IR* ir_;
  Version ir_version_;

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
