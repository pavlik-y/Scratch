#ifndef DIAG_H_
#define DIAG_H_

#include <Arduino.h>

#include "common.h"
#include "component.h"
#include "gyro.h"
#include "sensor_fusion.h"
#include "tilt_controller.h"

static const int kDataSize = 126;
static const int kInterval = 50;

#define RECORDED_VALUES(F) \
  F(sensor_fusion_->complementary_angle) \
  F(sensor_fusion_->gyro_rate) \
  F(tilt_controller_->power)
  


class Diag : public Component {
public:
  void Setup(SensorFusion* sensor_fusion, TiltController* tilt_controller) {
    sensor_fusion_ = sensor_fusion;
    tilt_controller_ = tilt_controller;
    recording_ = false;
    record_size_ = 0;
#define COUNT_RECORDS(value) ++record_size_;
    RECORDED_VALUES(COUNT_RECORDS)
  }
  
  void RecordSample(unsigned long value_index) {
#define RECORD_SAMPLE(value) data_[value_index % kDataSize] = value; ++value_index;
    RECORDED_VALUES(RECORD_SAMPLE)
  }

  virtual void Update() {
    if (!recording_)
      return;
    unsigned long index = millis() / kInterval;
    if (index_ == index)
      return;
    index_ = index;
    unsigned long value_index = index_ * record_size_;
    RecordSample(value_index);
  }
  
  virtual bool HandleCommand(CommandBuffer& cb) {
    if (strcmp_P(cb.command, PSTR("DiagRecord")) == 0) {
      recording_ = (cb.GetIntParam(0) != 0);
      cb.BeginResponse();
      cb.WriteValue(recording_);
      cb.EndResponse();
      return true;
    }
    if (strcmp_P(cb.command, PSTR("DiagDump")) == 0) {
      int index = cb.GetIntParam(0);
      cb.BeginResponse();
      unsigned long value_index = (index_ + 1 + index) * record_size_;
      cb.WriteValue((int)(value_index % kDataSize));
      for (int i = 0; i < record_size_; i++) 
        cb.WriteValue(data_[(value_index + i) % kDataSize]);
      cb.EndResponse();
      return true;
    }
    return false;
  }
  
private:
  SensorFusion* sensor_fusion_;
  TiltController* tilt_controller_;
  double data_[kDataSize];
  unsigned long index_;
  int record_size_;
  bool recording_;
};

#endif
