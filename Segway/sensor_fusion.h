#ifndef SENSOR_FUSION_H_
#define SENSOR_FUSION_H_

#include <Arduino.h>
#include "gyro.h"
#include "accel.h"
#include "command_buffer.h"
#include "common.h"
#include "component.h"
#include "config.h"
#include "motor.h"

class SensorFusion : public Component {
public:
  void Setup(Gyro* gyro, Accel* accel) {
    gyro_ = gyro;
    accel_ = accel;
    last_sample_time_ = millis();
    gyro_angle = 0;
    avg_accel_angle = 0;
    complementary_angle = 0;
    version = 0;
    lambda_ = 0;
  }
  
  virtual void Update() {
    if (gyro_version_ != gyro_->version) {
      gyro_version_ = gyro_->version;
      UpdateAngle();
      ++version;
    }
  }
  
  virtual void ReadConfig(Config* config) {
    lambda_ = config->ReadFloat_P(kSF_Lambda);
  }
  
  void UpdateAngle() {
    gyro_rate = gyro_->rate;
    accel_angle = accel_->angle;
    avg_accel_angle = LowPassFilter(avg_accel_angle, accel_angle, 0.1);
    double dt = double(millis() - last_sample_time_) / 1000.0;
    if (abs(gyro_angle - avg_accel_angle) > 90)
      gyro_angle = accel_angle;
    gyro_angle += gyro_rate * dt;
    complementary_angle += (accel_angle - complementary_angle) * lambda_ + gyro_rate * dt * (1.0 - lambda_);
    last_sample_time_ = millis();
  }

  virtual bool HandleCommand(CommandBuffer& cb) {
    if (strcmp_P(cb.command, PSTR("RdSF")) == 0) {
      cb.BeginResponse();
      cb.WriteValue(gyro_angle);
      cb.WriteValue(accel_angle);
      cb.WriteValue(complementary_angle);
      cb.EndResponse();
      return true;
    }
    return false;
  }
  
  void SynchronizeAngles(double angle) {
    gyro_angle = angle;
    complementary_angle = angle;
  }

  Version version;
  double gyro_rate;
  double gyro_angle;
  double accel_angle;
  double complementary_angle;
  double avg_accel_angle;
  
private:
  Gyro* gyro_;
  Version gyro_version_;
  Accel* accel_;
  double lambda_;
  unsigned long last_sample_time_;
};

#endif
