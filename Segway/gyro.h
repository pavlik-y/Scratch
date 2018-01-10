#ifndef GYRO_H_
#define GYRO_H_

#include <Arduino.h>
#include "common.h"
#include "component.h"

class Config;
class SensorChip;

class Gyro : public Component {
 public:
  Gyro();
  void Setup(SensorChip* sensors);
  void Update() override;
  bool HandleCommand(CommandBuffer& cb) override;
  void ReadConfig(Config* config) override;

  void SetBias(double bias);

  Version version;
  double rate;
  long raw_angle;
  long sample_count;

 private:
  // Reads rotation rate around y axis from gyro. Output is in deg/sec.
  void ReadSample();

  SensorChip* sensors_;

  double bias_;
  short raw_rate_;
};

#endif  // GYRO_H_
