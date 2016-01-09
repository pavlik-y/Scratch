#ifndef KALMAN_ADAPTER_H_
#define KALMAN_ADAPTER_H_

#include <Arduino.h>
#include "gyro.h"
#include "accel.h"
#include "command_buffer.h"
#include "kalman.h"
#include "storage.h"

class KalmanAdapter {
public:
  void Setup(Kalman* kalman, Gyro* gyro, Accel* accel) {
    kalman_ = kalman;
    gyro_ = gyro;
    accel_ = accel;
    last_sample_time_ = millis();
    double qAngle = Storage::ReadFloat(Storage::KALMAN_Q_ANGLE);
    double qBias = Storage::ReadFloat(Storage::KALMAN_Q_BIAS);
    double rMeasure = Storage::ReadFloat(Storage::KALMAN_R_MEASURE);
    kalman_->setQangle(qAngle);
    kalman_->setQbias(qBias);
    kalman_->setRmeasure(rMeasure);
    gyro_angle = 0;
    complementary_angle = 0;
  }
  
  void UpdateAngle() {
    gyro_rate = gyro_->rate;
    accel_angle = accel_->GetAngle();
    double deltaT = double(millis() - last_sample_time_) / 1000.0;
    if (abs(gyro_angle) > 180)
      gyro_angle = 0;
    kalman_angle = kalman_->getAngle(accel_angle, gyro_rate, deltaT);
    kalman_rate = kalman_->getRate();
    gyro_angle += gyro_rate * deltaT;
    double lambda = 0.01;
    complementary_angle += (accel_angle - complementary_angle) * lambda + gyro_rate * deltaT * (1.0-lambda);
    last_sample_time_ = millis();
  }

  double gyro_rate;
  double gyro_angle;
  double accel_angle;
  double kalman_angle;
  double kalman_rate;
  double complementary_angle;
  
  bool HandleCommand(CommandBuffer& cb) {
    if (cb.command == "RdKalman") {
      cb.BeginResponse();
      cb.WriteValue(gyro_angle);
      cb.WriteValue(accel_angle);
      cb.WriteValue(kalman_angle);
      cb.WriteValue(complementary_angle);
      cb.EndResponse();
      return true;
    }
    if (cb.command == "KalmanRdParams") {
      cb.BeginResponse();
      cb.WriteValue(kalman_->getQangle());
      cb.WriteValue(kalman_->getQbias());
      cb.WriteValue(kalman_->getRmeasure());
      cb.EndResponse();
      return true;
    }
    if (cb.command == "KalmanWrParams") {
      double qAngle = cb.GetFloatParam(0);
      double qBias = cb.GetFloatParam(1);
      double rMeasure = cb.GetFloatParam(2);
      kalman_->setQangle(qAngle);
      kalman_->setQbias(qBias);
      kalman_->setRmeasure(rMeasure);
      Storage::WriteFloat(Storage::KALMAN_Q_ANGLE, qAngle);
      Storage::WriteFloat(Storage::KALMAN_Q_BIAS, qBias);
      Storage::WriteFloat(Storage::KALMAN_R_MEASURE, rMeasure);
      cb.BeginResponse();
      cb.EndResponse();
      return true;
    }

    return false;
  }

private:
  Kalman* kalman_;
  Gyro* gyro_;
  Accel* accel_;
  unsigned long last_sample_time_;
};

#endif
