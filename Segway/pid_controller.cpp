#include "pid_controller.h"

PidController::PidController()
    : ie(0.0),
      lambda(0.1),
      Kp(0.3),
      Ki(0.3),
      Kd(0.3),
      setpoint(0.0) {}

void PidController::CalcOutput(
    double value, double error_delta, double time_delta) {
  double error = value - setpoint;
  ie += error * time_delta;
  if (ie > lambda)
    ie = lambda;
  else if (ie < -lambda)
    ie = -lambda;
//    ie += lambda * (error * time_delta - ie);
  output = Kp * error + Ki * ie + Kd * error_delta;
}

void PidController::SetCoefficients(
    double Kp_, double Ki_, double Kd_, double lambda_) {
  Kp = Kp_;
  Ki = Ki_;
  Kd = Kd_;
  lambda = lambda_;
}

void PidController::SetSetpoint(double setpoint_) {
  setpoint = setpoint_;
}
