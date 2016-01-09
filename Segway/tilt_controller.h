#ifndef tilt_controller_h_
#define tilt_controller_h_

#include <Arduino.h>

#include "common.h"
#include "component.h"
#include "config.h"
#include "pid.h"
#include "potentiometer.h"
#include "sensor_fusion.h"
#include "velocity_controller.h"

class TiltController : public Component {
public:
  void Setup(SensorFusion* sensor_fusion, PidController* angle_to_power, VelocityController* velocity_controller, Potentiometer* potentiometer) {
    sensor_fusion_ = sensor_fusion;
    sensor_fusion_version_ = sensor_fusion_->version;
    velocity_controller_ = velocity_controller;
    velocity_controller_version_ = velocity_controller_->version;
    angle_to_power_ = angle_to_power;
    potentiometer_ = potentiometer;
    last_micros_ = micros();
    power = 0;
    version = 0;
  }
  
  void UpdatePidControllers() {
    unsigned long now_micros = micros();
//    double offset = (potentiometer_->value - 0.5) * 10.0;
    double offset = velocity_controller_->angle_offset;
    angle_to_power_->SetSetpoint(upright_angle_ + offset);

    angle_to_power_->CalcOutput(sensor_fusion_->complementary_angle, sensor_fusion_->gyro_rate, 0.0); // dt is 0. Tilt controller doesn't use integral component
    last_micros_ = now_micros;
    power = constrain(angle_to_power_->output, -1.0, 1.0);    
  }
  
  virtual void Update() {
    if (sensor_fusion_version_ == sensor_fusion_->version &&
        velocity_controller_version_ == velocity_controller_->version)
      return;
    UpdatePidControllers();
    sensor_fusion_version_ = sensor_fusion_->version;
    velocity_controller_version_ = velocity_controller_->version;
    ++version;
  }
  
  virtual void ReadConfig(Config* config) {
    upright_angle_ = config->ReadFloat_P(kBal_UprightAngle);
    double kp = config->ReadFloat_P(kTiltCtrl_KP);
    double ki = config->ReadFloat_P(kTiltCtrl_KI);
    double kd = config->ReadFloat_P(kTiltCtrl_KD);
    double lambda = config->ReadFloat_P(kTiltCtrl_KL);
    
    angle_to_power_->SetSetpoint(upright_angle_);
    angle_to_power_->SetCoefficients(kp, ki, kd, lambda);
  }
  
  virtual bool HandleCommand(CommandBuffer& cb) {
    if (strcmp_P(cb.command, PSTR("RdTilt")) == 0) {
      cb.BeginResponse();
      cb.WriteValue(power);
      cb.WriteValue(angle_to_power_->ie);
      cb.WriteValue(version);
      cb.WriteValue(angle_to_power_->setpoint);
      cb.EndResponse();
      return true;
    }
    return false;
  }

  double power;
  Version version;
  
private:
  SensorFusion* sensor_fusion_;
  Version sensor_fusion_version_;
  VelocityController* velocity_controller_;
  Version velocity_controller_version_;
  PidController* angle_to_power_;
  Potentiometer* potentiometer_;
  unsigned long last_micros_;
  double upright_angle_;
};

#endif
