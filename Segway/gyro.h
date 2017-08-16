#ifndef GYRO_H_
#define GYRO_H_

#include <Arduino.h>
#include "common.h"
#include "component.h"
#include "i2c_device.h"

class Config;
//  L3G4200D: http://www.pololu.com/file/0J491/L3G4200D.pdf
class Gyro : public Component {
 public:
  Gyro();
  void Setup();
  void Update() override;
  bool HandleCommand(CommandBuffer& cb) override;
  void ReadConfig(Config* config) override;

  void SetBias(double bias);

  Version version;
  double rate;
  long raw_angle;
  long sample_count;

 private:
  bool DataAvailable();

  // Reads rotation rate around y axis from gyro. Output is in deg/sec.
  void ReadSample();

  I2CDevice device_;
  // Reflects sensor scale. Multiply reading by this value to get deg/sec
  static const double factor_ = 8.75 / 1000.0;
  double bias_;
  short raw_rate_;
};

#endif  // GYRO_H_
