#ifndef SENSOR_FUSION_H_
#define SENSOR_FUSION_H_

#include <Arduino.h>

#include "command_buffer.h"
#include "common.h"
#include "component.h"

class Accel;
class CommandBuffer;
class Config;
class Gyro;

class SensorFusion : public Component {
public:
  void Setup(Gyro* gyro, Accel* accel);

  void Update() override;
  void ReadConfig(Config* config) override;
  bool HandleCommand(CommandBuffer& cb) override;

  Version version = 0;
  double gyro_rate;
  double accel_angle;
  double complementary_angle;

private:
  void UpdateAngle(unsigned long now);

  Gyro* gyro_;
  Version gyro_version_;
  Accel* accel_;

  double lambda_;
  unsigned long last_sample_time_micros_;
};

#endif  // SENSOR_FUSION_H_
