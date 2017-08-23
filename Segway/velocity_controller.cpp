#include "velocity_controller.h"

#include "command_buffer.h"
#include "config.h"
#include "ir.h"
#include "pid_controller.h"
#include "position.h"

void VelocityController::Setup(PidController* velocity_to_angle, Position* position, IR* ir) {
  velocity_to_angle_ = velocity_to_angle;
  position_ = position;
  position_version_ = position_->version;
  last_sample_time_ = position_->sample_time;
  ir_ = ir;
  ir_version_ = ir_->version;
  version = 0;
  velocity_to_angle_->SetSetpoint(0.0);
}

void VelocityController::Update() {
  if (position_version_ == position_->version &&
      ir_version_ == ir_->version)
    return;
  unsigned long now = position_->sample_time;
  double set_velocity = 0;
  if (ir_->command == IR::Forward)
    set_velocity = 30.0;
  else if(ir_->command == IR::Back)
    set_velocity = -30.0;
  velocity_to_angle_->SetSetpoint(set_velocity);
  velocity_to_angle_->CalcOutput(position_->velocity, 0, ElapsedTime(last_sample_time_, now));
  angle_offset = constrain(velocity_to_angle_->output, -5.0, 5.0);
  last_sample_time_ = now;
  position_version_ = position_->version;
  ++version;
}

bool VelocityController::HandleCommand(CommandBuffer& cb) {
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

void VelocityController::ReadConfig(Config* config) {
  double kp = config->ReadFloat_P(kVelCtrl_KP);
  double ki = config->ReadFloat_P(kVelCtrl_KI);
  double kd = config->ReadFloat_P(kVelCtrl_KD);
  double lambda = config->ReadFloat_P(kVelCtrl_KL);

  velocity_to_angle_->SetCoefficients(kp, ki, kd, lambda);
}
