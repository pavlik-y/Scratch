#ifndef PID_CONTROLLER_H_
#define PID_CONTROLLER_H_

class PidController {
public:
  PidController();

  void CalcOutput(double value, double error_delta, double time_delta);
  void SetCoefficients(double Kp_, double Ki_, double Kd_, double lambda_);
  void SetSetpoint(double setpoint_);

  double output;
  double ie;
  double lambda;
  double Kp;
  double Ki;
  double Kd;
  double setpoint;
};

#endif  // PID_CONTROLLER_H_
