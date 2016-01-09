#ifndef velocity_controller_h_
#define velocity_controller_h_

#include <Arduino.h>

#include "common.h"
#include "component.h"
#include "ir.h"
#include "position.h"
#include "potentiometer.h"

class VelocityController : public Component {
public:
  void Setup(PidController* velocity_to_angle, Position* position, Potentiometer* potentiometer, IR* ir) {
    velocity_to_angle_ = velocity_to_angle;
    position_ = position;
    position_version_ = position_->version;
    last_sample_time_ = position_->sample_time;
    potentiometer_ = potentiometer;
    ir_ = ir;
    ir_version_ = ir_->version;
    version = 0;
    velocity_to_angle_->SetSetpoint(0.0);
  }
  
  virtual void Update() {
    if (position_version_ == position_->version &&
        ir_version_ == ir_->version)
      return;
    unsigned long now = position_->sample_time;
    double set_velocity = 0;
    if (ir_->command == IR::Forward)
      set_velocity = 30.0;
    else if(ir_->command == IR::Back)
      set_velocity = -30.0;
//    velocity_to_angle_->SetSetpoint((potentiometer_->StepValue(3) - 1.0) * 30.0);
    velocity_to_angle_->SetSetpoint(set_velocity);
    velocity_to_angle_->CalcOutput(position_->velocity, 0, ElapsedTime(last_sample_time_, now));
    angle_offset = constrain(velocity_to_angle_->output, -5.0, 5.0);
    last_sample_time_ = now;
    position_version_ = position_->version;
    ++version;
  }
  
  virtual bool HandleCommand(CommandBuffer& cb) {
    if (strcmp_P(cb.command, PSTR("RdVelCtrl")) == 0) {
      cb.BeginResponse();
      cb.WriteValue(angle_offset);
      cb.WriteValue(velocity_to_angle_->ie);
      cb.WriteValue(velocity_to_angle_->setpoint);
      cb.EndResponse();
      return true;
    }
    return false;
  }
  
  virtual void ReadConfig(Config* config) {
    double kp = config->ReadFloat_P(kVelCtrl_KP);
    double ki = config->ReadFloat_P(kVelCtrl_KI);
    double kd = config->ReadFloat_P(kVelCtrl_KD);
    double lambda = config->ReadFloat_P(kVelCtrl_KL);
    
    velocity_to_angle_->SetCoefficients(kp, ki, kd, lambda);
  }
  
  Version version;
  double angle_offset;
  
private:
  Position* position_;
  Version position_version_;
  Potentiometer* potentiometer_;
  IR* ir_;
  Version ir_version_;
  PidController* velocity_to_angle_;
  unsigned long last_sample_time_;
  
};

#endif
