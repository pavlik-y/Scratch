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

  void UpdateAngle();
  void SynchronizeAngles(double angle);

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

#endif  // SENSOR_FUSION_H_
