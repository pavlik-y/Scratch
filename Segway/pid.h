#ifndef PID_H_
#define PID_H_

#include <Arduino.h>

#include "common.h"
#include "component.h"
#include "config.h"

class PidController {
public:
  PidController() {
    ie = 0.0;
    lambda = 0.1;
    Kp = 0.3;
    Ki = 0.3;
    Kd = 0.3;
    setpoint = 0.0;
  }
  
  void CalcOutput(double v, double de, double dt) {
    double e = v - setpoint;
    ie += e * dt;
    if (ie > lambda)
      ie = lambda;
    else if (ie < -lambda)
      ie = -lambda;
//    ie += lambda * (e * dt - ie);
    output = Kp * e + Ki * ie + Kd * de;
  }
  
  void SetCoefficients(double Kp_, double Ki_, double Kd_, double lambda_) {
    Kp = Kp_;
    Ki = Ki_;
    Kd = Kd_;
    lambda = lambda_;
  }
  
  void SetSetpoint(double setpoint_) {
    setpoint = setpoint_;
  }
  
  double output;
  double ie;
  double lambda;
  double Kp;
  double Ki;
  double Kd;
  double setpoint;
};

#endif
