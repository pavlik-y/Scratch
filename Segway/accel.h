#ifndef ACCEL_H_
#define ACCEL_H_

#include <Arduino.h>
#include "common.h"
#include "component.h"
#include "i2c_device.h"

class CommandBuffer;
class Config;
class Gyro;

// ADXL345: http://www.analog.com/static/imported-files/data_sheets/ADXL345.pdf
class Accel : public Component {
public:
  Accel();

  void Setup(Gyro* gyro);

  void Update() override;
  void ReadConfig(Config* config) override;
  bool HandleCommand(CommandBuffer& cb) override;

  short x;
  short z;
  double angle;
  Version version;

private:
  void ReadSample();

  static const double factor_ = 57.2957795131; // (1 / pi) * 180

  I2CDevice device_;
  short xBias_;
  short zBias_;

  Gyro* gyro_;
  Version gyro_version_;
};

#endif  // ACCEL_H_
