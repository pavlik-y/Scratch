#ifndef DIAGNOSTIC_H_
#define DIAGNOSTIC_H_

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "kalman_adapter.h"

struct Sample {
  double accel_angle;
  double gyro_angle;
  double kalman_angle;
};

class Diagnostic {
public:
  void Setup(SoftwareSerial* bt, KalmanAdapter* kalman_adapter) {
    bt_ = bt;
    kalman_adapter_ = kalman_adapter;
    sample_index_ = 0;
    collect_ = false;
  }
  
  void RecordSample() {
    if (!collect_)
      return;
    Sample* sample = samples_ + sample_index_;
    sample->accel_angle = kalman_adapter_->accel_angle;
    sample->gyro_angle = kalman_adapter_->gyro_angle;
    sample->kalman_angle = kalman_adapter_->kalman_angle;
    ++sample_index_;
    if (sample_index_ == SAMPLE_COUNT) {
      collect_ = false;
      DumpData();
    }
  }
  
  void DumpData() {
    for (int i = 0; i < sample_index_; ++i) {
      Sample* s = samples_ + i;
      StartRecord();
      WriteValue(s->accel_angle);
      WriteValue(s->gyro_angle);
      WriteValue(s->kalman_angle);
      EndRecord();
    }
  }
  
  void StartRecord() {
    bor_ = true;
  }
  
  void EndRecord() {
    bt_->println();
  }
  
  void WriteValue(int value) {
    if (!bor_)
      bt_->print(",");
    else
      bor_ = false;
    bt_->print(value);
  }
  
  void WriteValue(long value) {
    if (!bor_)
      bt_->print(",");
    else
      bor_ = false;
    bt_->print(value);
  }
  
  void WriteValue(double value) {
    if (!bor_)
      bt_->print(",");
    else
      bor_ = false;
    bt_->print(value);
  }

  bool HandleCommand(CommandBuffer& cb) {
    if (cb.command == "DiagCollect") {
      collect_ = true;
      sample_index_ = 0;
      cb.BeginResponse();
      cb.WriteValue("Ok");
      cb.EndResponse();
      return true;
    }
    return false;
  }
  
private:
  static const int SAMPLE_COUNT = 20;
  SoftwareSerial* bt_;
  KalmanAdapter* kalman_adapter_;

  Sample samples_[SAMPLE_COUNT];
  int sample_index_;
  bool collect_;
  bool bor_;
};

#endif
